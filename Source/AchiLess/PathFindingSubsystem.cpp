
#include "PathfindingSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Algo/Sort.h"

void UPathfindingSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);

    // シーン内に存在するASpaceOctreeActorを検索して参照を取得
    TArray<AActor*> FoundActors;
    // GetWorld() は UWorldSubsystem のメンバー関数なので直接呼び出し可
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

TArray<FVector> UPathfindingSubsystem::FindPath(const FVector& StartLocation, const FVector& EndLocation)
{
    TArray<FVector> Path;

    if (!SpaceOctree || !SpaceOctree->RootNode)
    {
        UE_LOG(LogTemp, Warning, TEXT("Octree is not initialized or not found. Cannot find path."));
        return Path;
    }

    // スタートノードとエンドノードを取得
    UOctreeNode* StartOctreeNode = GetOctreeNodeAtLocation(StartLocation);
    UOctreeNode* EndOctreeNode = GetOctreeNodeAtLocation(EndLocation);

    if (!StartOctreeNode || !EndOctreeNode)
    {
        UE_LOG(LogTemp, Warning, TEXT("Start or End Octree Node not found. Cannot find path."));
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
    TArray<UAStarNode*> OpenList;
    TMap<UOctreeNode*, UAStarNode*> ClosedList; // 既に評価済みのOctreeNodeとそれに対応するAStarNode

    // スタートノードを作成し、オープンリストに追加
    UAStarNode* StartAStarNode = NewObject<UAStarNode>();
    StartAStarNode->Initialize(StartOctreeNode, nullptr, 0.0f, CalculateHCost(StartLocation, EndLocation));
    OpenList.Add(StartAStarNode);

    // 各OctreeNodeに対応するAStarNodeを保持するマップ
    TMap<UOctreeNode*, UAStarNode*> AllAStarNodes;
    AllAStarNodes.Add(StartOctreeNode, StartAStarNode);

    while (OpenList.Num() > 0)
    {
        // OpenListからFコストが最小のノードを取得
        // Fコストが同じ場合はHコストが最小のノードを選択
        UAStarNode* CurrentAStarNode = OpenList[0];
        for (int32 i = 1; i < OpenList.Num(); ++i)
        {
            if (OpenList[i]->FCost < CurrentAStarNode->FCost ||
                (OpenList[i]->FCost == CurrentAStarNode->FCost && OpenList[i]->HCost < CurrentAStarNode->HCost))
            {
                CurrentAStarNode = OpenList[i];
            }
        }

        // OpenListから現在のノードを削除し、ClosedListに追加
        OpenList.Remove(CurrentAStarNode);
        ClosedList.Add(CurrentAStarNode->OctreeNode, CurrentAStarNode);

        // 目標ノードに到達した場合
        if (CurrentAStarNode->OctreeNode == EndOctreeNode)
        {
            return ReconstructPath(CurrentAStarNode);
        }

        // 隣接ノードを探索
        TArray<UOctreeNode*> Neighbors = GetNeighboringOctreeNodes(CurrentAStarNode->OctreeNode);
        for (UOctreeNode* NeighborOctreeNode : Neighbors)
        {
            // 隣接ノードが既にClosedListにあるか、障害物であればスキップ
            if (ClosedList.Contains(NeighborOctreeNode) || SpaceOctree->IsBoxBlocked(NeighborOctreeNode->Bounds))
            {
                continue;
            }

            // 隣接ノードまでのGコストを計算
            float NewGCost = CurrentAStarNode->GCost + FVector::Distance(CurrentAStarNode->GetCenter(), NeighborOctreeNode->Bounds.GetCenter());

            UAStarNode* NeighborAStarNode = AllAStarNodes.FindRef(NeighborOctreeNode);

            // 隣接ノードがOpenListにない、または新しい経路の方がコストが低い場合
            if (!NeighborAStarNode || NewGCost < NeighborAStarNode->GCost)
            {
                if (!NeighborAStarNode)
                {
                    NeighborAStarNode = NewObject<UAStarNode>();
                    AllAStarNodes.Add(NeighborOctreeNode, NeighborAStarNode);
                }
                NeighborAStarNode->Initialize(NeighborOctreeNode, CurrentAStarNode, NewGCost, CalculateHCost(NeighborOctreeNode->Bounds.GetCenter(), EndLocation));

                if (!OpenList.Contains(NeighborAStarNode))
                {
                    OpenList.Add(NeighborAStarNode);
                }
            }
        }
    }

    UE_LOG(LogTemp, Warning, TEXT("Path not found."));
    return Path; // 経路が見つからなかった場合
}

float UPathfindingSubsystem::CalculateHCost(const FVector& FromLocation, const FVector& ToLocation) const
{
    // マンハッタン距離またはユークリッド距離を使用
    return FVector::Distance(FromLocation, ToLocation); // ユークリッド距離
    // return FVector::DistManhattan(FromLocation, ToLocation); // マンハッタン距離
}

TArray<UOctreeNode*> UPathfindingSubsystem::GetNeighboringOctreeNodes(UOctreeNode* CurrentOctreeNode) const
{
    TArray<UOctreeNode*> Neighbors;
    if (!SpaceOctree || !SpaceOctree->RootNode) return Neighbors;

    // 現在のノードと隣接するOctreeノードを探すロジックを実装
    // これはOctreeの構造上、少し複雑になる可能性があります。
    // Simplest approach: Check immediate neighbors (parent and siblings, or children of parent)
    // より正確な方法: 現在のノードの周囲の空間をカバーするノードを探索する
    // ここでは簡易的に、現在のノードと同じ階層、または親ノードの他の子ノードを隣接ノードとして考慮します。
    // 厳密な3Dグリッドベースの隣接ノード取得とは異なります。

    // 例えば、現在のノードの親ノードの全ての子ノード（現在のノード自身を除く）を隣接ノードとみなす
    // または、現在のノードの周囲の8方向にあるであろうノードを探索する
    // Octreeの性質上、隣接ノードのサイズが異なる可能性もあります。

    // 現実的な実装としては、特定の半径内のOctreeノードを検索する、
    // または、現在のノードの隣接する空間をカバーするノードを見つけるための関数をOctreeクラスに追加する、
    // というアプローチが考えられます。

    // ここでは、現在のノードの中心から隣接する空間にA*探索のステップサイズ（例えばMinNodeSize）を足し引きして、
    // その位置にあるOctreeノードを取得する方法を実装します。

    float Step = SpaceOctree->MinNodeSize; // ノード間のステップサイズとして最小ノードサイズを使用
    FVector CurrentCenter = CurrentOctreeNode->Bounds.GetCenter();

    // 26方向（自分自身を除く）の隣接位置
    for (int x = -1; x <= 1; ++x)
    {
        for (int y = -1; y <= 1; ++y)
        {
            for (int z = -1; z <= 1; ++z)
            {
                if (x == 0 && y == 0 && z == 0) continue; // 自分自身はスキップ

                FVector NeighborLocation = CurrentCenter + FVector(x * Step, y * Step, z * Step);
                UOctreeNode* NeighborNode = GetOctreeNodeAtLocation(NeighborLocation);
                if (NeighborNode && NeighborNode != CurrentOctreeNode && !Neighbors.Contains(NeighborNode))
                {
                    Neighbors.Add(NeighborNode);
                }
            }
        }
    }

    return Neighbors;
}

TArray<FVector> UPathfindingSubsystem::ReconstructPath(UAStarNode* EndAStarNode) const
{
    TArray<FVector> Path;
    UAStarNode* Current = EndAStarNode;
    while (Current)
    {
        Path.Add(Current->GetCenter());
        Current = Current->Parent;
    }
    Algo::Reverse(Path); // 経路を逆転させてスタートからエンドの順にする

    return Path;
}

UOctreeNode* UPathfindingSubsystem::GetOctreeNodeAtLocation(const FVector& Location) const
{
    if (!SpaceOctree || !SpaceOctree->RootNode) return nullptr;

    UOctreeNode* CurrentNode = SpaceOctree->RootNode;
    while (CurrentNode)
    {
        if (CurrentNode->bContainsObstacle) return nullptr; // 障害物ノードであれば経路として利用できない

        if (!CurrentNode->HasChildren())
        {
            // 葉ノードに到達し、ブロックされていなければそのノードを返す
            if (CurrentNode->Bounds.IsInside(Location))
            {
                return CurrentNode;
            }
            return nullptr; // 葉ノードだが、位置が含まれていない
        }

        // 子ノードをチェックし、位置が含まれる子ノードへ移動
        UOctreeNode* NextNode = nullptr;
        for (UOctreeNode* Child : CurrentNode->Children)
        {
            if (Child && Child->Bounds.IsInside(Location))
            {
                NextNode = Child;
                break;
            }
        }

        if (NextNode)
        {
            CurrentNode = NextNode;
        }
        else
        {
            return nullptr; // どのノードにも含まれない (RootNodeの範囲外など)
        }
    }
    return nullptr;
}

