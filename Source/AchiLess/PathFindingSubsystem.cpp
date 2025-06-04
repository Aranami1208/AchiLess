#include "PathfindingSubsystem.h"
#include "Kismet/GameplayStatics.h"
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
    SpaceOctree = Ref;
}


TArray<FVector> UPathfindingSubsystem::FindPath(const FVector& StartLocation, const FVector& EndLocation)
{
    TArray<FVector> Path;

    if (!SpaceOctree || SpaceOctree->RootNodeIndex == INDEX_NONE)
    {
        UE_LOG(LogTemp, Warning, TEXT("Octree is not initialized or not found. Cannot find path."));
        return Path;
    }

    // A*探索用の全ノードをクリア
    AllAStarNodes.Empty();

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
    TArray<int32> OpenListIndices; // OpenListもインデックスで管理
    TMap<int32, int32> ClosedList; // OctreeNodeIndex -> FAStarNodeIndex

    // スタートFAStarNodeを作成し、AllAStarNodesに追加
    FAStarNode StartAStarNode;
    StartAStarNode.Initialize(StartOctreeNodeIndex, INDEX_NONE, 0.0f, CalculateHCost(GetAStarNodeCenter(AllAStarNodes.Num()), EndLocation));
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
            return ReconstructPath(CurrentAStarNodeIndex);
        }

        // 現在のOctreeNodeを取得
        const FOctreeNode* CurrentOctreeNode = SpaceOctree->GetNode(AllAStarNodes[CurrentAStarNodeIndex].OctreeNodeIndex);
        if (!CurrentOctreeNode) continue;

        // 隣接ノードを探索
        TArray<int32> NeighborsOctreeIndices = GetNeighboringOctreeNodes(AllAStarNodes[CurrentAStarNodeIndex].OctreeNodeIndex);
        for (int32 NeighborOctreeNodeIndex : NeighborsOctreeIndices)
        {
            if (NeighborOctreeNodeIndex == INDEX_NONE || ClosedList.Contains(NeighborOctreeNodeIndex))
            {
                continue;
            }

            const FOctreeNode* NeighborOctreeNode = SpaceOctree->GetNode(NeighborOctreeNodeIndex);
            if (!NeighborOctreeNode) continue;

            if (SpaceOctree->IsBoxBlocked(NeighborOctreeNode->Bounds))
            {
                continue;
            }

            // 隣接ノードまでのGコストを計算
            float NewGCost = AllAStarNodes[CurrentAStarNodeIndex].GCost + FVector::Distance(CurrentOctreeNode->Bounds.GetCenter(), NeighborOctreeNode->Bounds.GetCenter());

            int32 NeighborAStarNodeIndex = OctreeNodeIndexToAStarNodeIndexMap.FindRef(NeighborOctreeNodeIndex);

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

float UPathfindingSubsystem::CalculateHCost(const FVector& FromLocation, const FVector& ToLocation) const
{
    return FVector::Distance(FromLocation, ToLocation);
}

TArray<int32> UPathfindingSubsystem::GetNeighboringOctreeNodes(int32 CurrentOctreeNodeIndex) const
{
    TArray<int32> Neighbors;
    if (!SpaceOctree || CurrentOctreeNodeIndex == INDEX_NONE) return Neighbors;

    const FOctreeNode* CurrentOctreeNode = SpaceOctree->GetNode(CurrentOctreeNodeIndex);
    if (!CurrentOctreeNode) return Neighbors;

    float Step = SpaceOctree->MinNodeSize;
    FVector CurrentCenter = CurrentOctreeNode->Bounds.GetCenter();

    for (int x = -1; x <= 1; ++x)
    {
        for (int y = -1; y <= 1; ++y)
        {
            for (int z = -1; z <= 1; ++z)
            {
                if (x == 0 && y == 0 && z == 0) continue;

                FVector NeighborLocation = CurrentCenter + FVector(x * Step, y * Step, z * Step);
                FOctreeNode NeighborNode = SpaceOctree->GetOctreeNodeAtLocation(NeighborLocation); // FOctreeNodeを値で取得
                if (NeighborNode.Bounds.IsValid) // 有効なノードが取得できたかチェック
                {
                    int32 NeighborNodeIndex = SpaceOctree->AllNodes.Find(NeighborNode);
                    if (NeighborNodeIndex != INDEX_NONE && NeighborNodeIndex != CurrentOctreeNodeIndex && !Neighbors.Contains(NeighborNodeIndex))
                    {
                        Neighbors.Add(NeighborNodeIndex);
                    }
                }
            }
        }
    }

    return Neighbors;

}

TArray<FVector> UPathfindingSubsystem::ReconstructPath(int32 EndAStarNodeIndex) const
{
    TArray<FVector> Path;
    int32 CurrentAStarNodeIndex = EndAStarNodeIndex;
    while (CurrentAStarNodeIndex != INDEX_NONE)
    {
        const FAStarNode& CurrentNode = AllAStarNodes[CurrentAStarNodeIndex];
        Path.Add(GetAStarNodeCenter(CurrentAStarNodeIndex)); // ヘルパー関数を使用
        CurrentAStarNodeIndex = CurrentNode.ParentIndex;
    }
    Algo::Reverse(Path);

    return Path;
}



FVector UPathfindingSubsystem::GetAStarNodeCenter(int32 AStarNodeIndex) const
{
    if (AllAStarNodes.IsValidIndex(AStarNodeIndex) && SpaceOctree && SpaceOctree->AllNodes.IsValidIndex(AllAStarNodes[AStarNodeIndex].OctreeNodeIndex))
    {
        return SpaceOctree->AllNodes[AllAStarNodes[AStarNodeIndex].OctreeNodeIndex].Bounds.GetCenter();
    }
    return FVector::ZeroVector;
}