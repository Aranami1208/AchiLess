#include "PathfindingSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Async/Async.h"
#include "Algo/Sort.h"

void UPathfindingSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);

    TArray<AActor*> FoundActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpaceOctree::StaticClass(), FoundActors);
    if (FoundActors.Num() > 0)
    {
        SpaceOctree = Cast<ASpaceOctree>(FoundActors[0]);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("ASpaceOctree Actor not found in the world. Pathfinding will not work."));
    }
}

void UPathfindingSubsystem::Deinitialize()
{
    SpaceOctree = nullptr;
    Super::Deinitialize();
}

void UPathfindingSubsystem::SetOctreeReference(ASpaceOctree* Ref)
{
    if (!Ref)return;
    SpaceOctree = Ref;

    bIsOctreeInitialized = true;
    OnOctreeReady.Broadcast();
}


TArray<FVector> UPathfindingSubsystem::FindPath(const FVector& StartLocation, const FVector& EndLocation)
{
    TArray<FVector> Path;

    if (!SpaceOctree || SpaceOctree->RootNodeIndex == INDEX_NONE)
    {
        UE_LOG(LogTemp, Warning, TEXT("Octree is not initialized or not found. Cannot find path."));
        return Path;
    }

    

    // スタートノードとエンドノードのOctreeNodeデータを取得
    FOctreeNode StartOctreeNodeData = SpaceOctree->GetOctreeNodeAtLocation(StartLocation);
    FOctreeNode EndOctreeNodeData = SpaceOctree->GetOctreeNodeAtLocation(EndLocation);

    if (!StartOctreeNodeData.Bounds.IsValid || !EndOctreeNodeData.Bounds.IsValid)
    {
        UE_LOG(LogTemp, Warning, TEXT("Start or End Octree Node data is invalid. Cannot find path."));
        return Path;
    }

    // スタートノードのOctreeNodeIndexとエンドノードのOctreeNodeIndexを取得
    int32 StartOctreeNodeIndex = SpaceOctree->AllNodes.Find(StartOctreeNodeData);
    int32 EndOctreeNodeIndex = SpaceOctree->AllNodes.Find(EndOctreeNodeData);

    if (StartOctreeNodeIndex == INDEX_NONE || EndOctreeNodeIndex == INDEX_NONE)
    {
        UE_LOG(LogTemp, Warning, TEXT("Could not find index for Start or End Octree Node data."));
        return Path;
    }

    // スタートノードが障害物の場合、経路は存在しない
    if (SpaceOctree->IsLocationBlocked(StartLocation))
    {
        UE_LOG(LogTemp, Warning, TEXT("Start location is blocked. Cannot find path."));
        return Path;
    }
    // エンドノードが障害物の場合、経路は存在しない
    if (SpaceOctree->IsLocationBlocked(EndLocation))
    {
        UE_LOG(LogTemp, Warning, TEXT("End location is blocked. Cannot find path."));
        return Path;
    }

    // A*アルゴリズムの実装
      // A*アルゴリズムの全ノードを格納する配列
   // FAStarNodeはUSTRUCTなので、直接配列に格納できる
    TArray<FAStarNode> AllAStarNodes;

    TArray<int32> OpenListIndices; // OpenListもインデックスで管理
    TMap<int32, int32> ClosedList; // OctreeNodeIndex -> FAStarNodeIndex

    // スタートFAStarNodeを作成し、AllAStarNodesに追加
    const FVector StartNodeCenter = SpaceOctree->GetNode(StartOctreeNodeIndex)->Bounds.GetCenter();

    FAStarNode StartAStarNode;
    StartAStarNode.Initialize(StartOctreeNodeIndex, INDEX_NONE, 0.0f, CalculateHCost(StartNodeCenter, EndLocation));
    int32 StartAStarNodeIndex = AllAStarNodes.Add(StartAStarNode);

    OpenListIndices.Add(StartAStarNodeIndex);

    // OctreeNodeIndex -> FAStarNodeIndex のマップ (既に見つけたノードへのポインタの代わりにインデックスを格納)
    TMap<int32, int32> OctreeNodeIndexToAStarNodeIndexMap;
    OctreeNodeIndexToAStarNodeIndexMap.Add(StartOctreeNodeIndex, StartAStarNodeIndex);


    while (OpenListIndices.Num() > 0)
    {
        // OpenListからFコストが最小のノードのインデックスを取得
        int32 CurrentAStarNodeIndex = OpenListIndices[0];
        for (int32 i = 1; i < OpenListIndices.Num(); ++i)
        {
            if (AllAStarNodes[OpenListIndices[i]].FCost < AllAStarNodes[CurrentAStarNodeIndex].FCost ||
                (AllAStarNodes[OpenListIndices[i]].FCost == AllAStarNodes[CurrentAStarNodeIndex].FCost && AllAStarNodes[OpenListIndices[i]].HCost < AllAStarNodes[CurrentAStarNodeIndex].HCost))
            {
                CurrentAStarNodeIndex = OpenListIndices[i];
            }
        }

        // OpenListから現在のノードのインデックスを削除し、ClosedListに追加
        OpenListIndices.Remove(CurrentAStarNodeIndex);
        ClosedList.Add(AllAStarNodes[CurrentAStarNodeIndex].OctreeNodeIndex, CurrentAStarNodeIndex);

        // 目標ノードに到達した場合
        if (AllAStarNodes[CurrentAStarNodeIndex].OctreeNodeIndex == EndOctreeNodeIndex)
        {
            return ReconstructPath(CurrentAStarNodeIndex,AllAStarNodes);
        }

        // 現在のOctreeNodeを取得
        const FOctreeNode* CurrentOctreeNode = SpaceOctree->GetNode(AllAStarNodes[CurrentAStarNodeIndex].OctreeNodeIndex);
        if (!CurrentOctreeNode) continue;

        // 隣接ノードを探索
        TArray<int32> NeighborsOctreeIndices = GetNeighboringOctreeNodes(AllAStarNodes[CurrentAStarNodeIndex].OctreeNodeIndex);
        for (int32 NeighborOctreeNodeIndex : NeighborsOctreeIndices)
        {
            

            const FOctreeNode* NeighborOctreeNode = SpaceOctree->GetNode(NeighborOctreeNodeIndex);
            if (!NeighborOctreeNode) continue;

            if (SpaceOctree->IsBoxBlocked(NeighborOctreeNode->Bounds))
            {
                continue;
            }

            // 隣接ノードまでのGコストを計算
            float NewGCost = AllAStarNodes[CurrentAStarNodeIndex].GCost + FVector::Distance(CurrentOctreeNode->Bounds.GetCenter(), NeighborOctreeNode->Bounds.GetCenter());

            int32 NeighborAStarNodeIndex = OctreeNodeIndexToAStarNodeIndexMap.FindRef(NeighborOctreeNodeIndex);

            if (NeighborsOctreeIndices.Num() == 0 && OpenListIndices.Num() == 0)
            {
                UE_LOG(LogTemp, Warning, TEXT("A* Failure: No neighbors found and OpenList is empty. Path is impossible."));
            }

            // 隣接ノードがOpenListにない、または新しい経路の方がコストが低い場合
            if (NeighborAStarNodeIndex == 0 || NewGCost < AllAStarNodes[NeighborAStarNodeIndex].GCost) // 0はFindRefがデフォルト値を返す場合
            {
                if (NeighborAStarNodeIndex == 0) // まだFAStarNodeが作成されていない場合
                {
                    FAStarNode NewNeighborAStarNode;
                    NewNeighborAStarNode.Initialize(NeighborOctreeNodeIndex, CurrentAStarNodeIndex, NewGCost, CalculateHCost(NeighborOctreeNode->Bounds.GetCenter(), EndLocation));
                    NeighborAStarNodeIndex = AllAStarNodes.Add(NewNeighborAStarNode);
                    OctreeNodeIndexToAStarNodeIndexMap.Add(NeighborOctreeNodeIndex, NeighborAStarNodeIndex);
                }
                else
                {
                    // 既存のFAStarNodeを更新
                    AllAStarNodes[NeighborAStarNodeIndex].Initialize(NeighborOctreeNodeIndex, CurrentAStarNodeIndex, NewGCost, CalculateHCost(NeighborOctreeNode->Bounds.GetCenter(), EndLocation));
                }

                if (!OpenListIndices.Contains(NeighborAStarNodeIndex))
                {
                    OpenListIndices.Add(NeighborAStarNodeIndex);
                }
            }
        }
    }

    UE_LOG(LogTemp, Warning, TEXT("Path not found."));
    return Path; // 経路が見つからなかった場合
}

void UPathfindingSubsystem::FindPathAsync(const FVector& StartLocation, const FVector& EndLocation, const FOnPathfindingComplete& OnCompleteCallback)
{
    //すでに処理している場合は処理しない
    if (bIsPathFinding)return;

    bIsPathFinding = true;

    if (!SpaceOctree)
    {
        // エラー処理：SpaceOctreeが無効な場合もフラグを戻して終了
        OnCompleteCallback.ExecuteIfBound(TArray<FVector>());
        bIsPathFinding = false;
        return;
    }

    UPathfindingSubsystem* Subsystem = this;

    // ワーカースレッドで実行する処理をラムダ式で定義
    Async(EAsyncExecution::ThreadPool, [this,Subsystem, StartLocation, EndLocation, OnCompleteCallback]()
        {
            // --- ここからワーカースレッドでの処理 ---

            // 経路を計算
            TArray<FVector> ResultPath = Subsystem->FindPath(StartLocation, EndLocation);

            // --- ここまでワーカースレッドでの処理 ---


            // 計算結果をゲームスレッドに返して、コールバックを実行
            AsyncTask(ENamedThreads::GameThread, [this,OnCompleteCallback, ResultPath]()
                {
                    // --- ここからゲームスレッドでの処理 ---

                    // OnCompleteCallback デリゲートを実行して、結果を呼び出し元に通知
                    OnCompleteCallback.ExecuteIfBound(ResultPath);
                    this->bIsPathFinding = false;//処理終了
                    // --- ここまでゲームスレッドでの処理 ---
                });
        });
}

bool UPathfindingSubsystem::IsOctreeReady()
{
    return bIsOctreeInitialized;
}

float UPathfindingSubsystem::CalculateHCost(const FVector& FromLocation, const FVector& ToLocation) const
{
    return FVector::Distance(FromLocation, ToLocation);
}

TArray<int32> UPathfindingSubsystem::GetNeighboringOctreeNodes(int32 CurrentOctreeNodeIndex) const
{
    //隣接ノードのインデックス
    TArray<int32> Neighbors;

    //ツリーやノードのインデックスがない場合は処理しない
    if (!SpaceOctree || CurrentOctreeNodeIndex == INDEX_NONE) return Neighbors;

    //現在のノードをインデックスから取得
    const FOctreeNode* CurrentOctreeNode = SpaceOctree->GetNode(CurrentOctreeNodeIndex);
    //ノードが見つからなかった場合は処理しない
    if (!CurrentOctreeNode) return Neighbors;

    // 現在のノードの中心と半分のサイズを取得
    FVector CurrentCenter = CurrentOctreeNode->Bounds.GetCenter();
    FVector CurrentExtent = CurrentOctreeNode->Bounds.GetExtent();

    //自分のサイズ分移動しても、自分自身のノードを取得してしまうので、少しだけ外側に移動する分のオフセット
    const float QueryOffset = 1.0f; // わずかなオフセット値 (小さくても0ではない値)

    // 隣接する8方向（現在のノードから見た相対位置）を反復処理
    // 6つの面、12のエッジ、8つのコーナーに対応する方向ベクトルを生成
    // このループは26方向の隣接ノードをカバーしようと試みる
    for (int x = -1; x <= 1; ++x)
    {
        for (int y = -1; y <= 1; ++y)
        {
            for (int z = -1; z <= 1; ++z)
            {
                if (x == 0 && y == 0 && z == 0) continue; // 自分自身はスキップ

                // 隣接ノードを探索する位置を計算
                // CurrentExtentを基にすることで、ノードのサイズに応じた移動を試みる
                FVector NeighborQueryLocation = CurrentCenter +
                    FVector(x * (CurrentExtent.X + QueryOffset),
                        y * (CurrentExtent.Y + QueryOffset),
                        z * (CurrentExtent.Z + QueryOffset));

                // その位置にあるOctreeノードを取得
                FOctreeNode NeighborNodeData = SpaceOctree->GetOctreeNodeAtLocation(NeighborQueryLocation);

                // 取得したノードが有効かつ、現在のノードと異なり、かつまだリストに追加されていない場合
                if (static_cast<bool>(NeighborNodeData.Bounds.IsValid))
                {
                    int32 NeighborNodeIndex = SpaceOctree->AllNodes.Find(NeighborNodeData);
                    if (NeighborNodeIndex != INDEX_NONE &&
                        NeighborNodeIndex != CurrentOctreeNodeIndex &&
                        !Neighbors.Contains(NeighborNodeIndex))
                    {
                        Neighbors.Add(NeighborNodeIndex);
                    }
                }
            }
        }
    }

    return Neighbors;

}

TArray<FVector> UPathfindingSubsystem::ReconstructPath(int32 EndAStarNodeIndex, TArray<FAStarNode>& InAStarNodes) const
{
    TArray<FVector> Path;
    int32 CurrentAStarNodeIndex = EndAStarNodeIndex;
    while (CurrentAStarNodeIndex != INDEX_NONE)
    {
        const FAStarNode& CurrentNode = InAStarNodes[CurrentAStarNodeIndex];

        const int32 OctreeNodeIndex = CurrentNode.OctreeNodeIndex;
        if (SpaceOctree && SpaceOctree->AllNodes.IsValidIndex(CurrentAStarNodeIndex))
        {
            Path.Add(SpaceOctree->AllNodes[OctreeNodeIndex].Bounds.GetCenter()); // ヘルパー関数を使用
        }
        CurrentAStarNodeIndex = CurrentNode.ParentIndex;
    }
    Algo::Reverse(Path);

    return Path;
}
