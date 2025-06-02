// Fill out your copyright notice in the Description page of Project Settings.


#include "Octree.h"
#include "OctreeNode.h"

UOctree::UOctree()
	:MaxHitObjectPerNode(5.0f),MaxOctreeDepth(8),PawnSize(1800.0f)
{
}

UOctree::~UOctree()
{
	ClearOctree();
}



void UOctree::BuildOctree(const FBox& WorldBounds, const TArray<AActor*>& AllObstacles)
{
	ClearOctree();
	RootNode = MakeUnique<OctreeNode>(WorldBounds, 0);
	BuildNodeRecursive(RootNode.Get(), AllObstacles);
}

void UOctree::ClearOctree()
{
    //処理追加　
}

void UOctree::UpdateDynamicObstacles(const TArray<AActor*>& DynamicObstacles)
{
    //処理追加
}

bool UOctree::IsPathClear(const FVector& StartLocation, const FVector& EndLocation) const
{
    if (!RootNode.IsValid()) return true; // ツリーがない場合はクリア

    // 飛行機（AI）のコリジョン形状を近似するボックス
    // 飛行機の向きを考慮しない場合、StartLocationとEndLocationを包むAABB + AIの半径/半幅
    FBox SweepBox;
    SweepBox += StartLocation;
    SweepBox += EndLocation;
    SweepBox = SweepBox.ExpandBy(PawnSize / 2.0f); // AIの半径分広げる

    return QueryNodeRecursive(RootNode.Get(), StartLocation, EndLocation, SweepBox);

}

void UOctree::BuildNodeRecursive(OctreeNode* CurrentNode, const TArray<AActor*>& ObstaclesInNode)
{
    // このノードに障害物を追加
    for (AActor* Obstacle : ObstaclesInNode)
    {
        CurrentNode->AddHitObject(Obstacle);
    }

    // ノードがAIのサイズに対してブロックされているかを判定 (初期判定)
    // ここで、ノードのBoundsの対角線長がAIのサイズより小さい場合なども考慮できる
    CurrentNode->UpdateNodeStatus(PawnSize);

    // 分割条件をチェック (実装はOctreeNode::ShouldSubdivideに依存)
    if (CurrentNode->ShouldSubdivide(MaxHitObjectPerNode, MaxOctreeDepth))
    {
        CurrentNode->Subdivide(); // 8つの子ノードを生成
        for (const TUniquePtr<OctreeNode>& Child : CurrentNode->Children)
        {
            TArray<AActor*> ChildObstacles;
            // この子ノードのBoundsと交差する障害物のみを抽出
            for (AActor* Obstacle : ObstaclesInNode)
            {
                if (Obstacle && Obstacle->IsValidLowLevel() && Obstacle->GetRootComponent())
                {
                    // 障害物のコリジョンコンポーネントのBoundsを取得
                    UPrimitiveComponent* PrimitiveComp = Cast<UPrimitiveComponent>(Obstacle->GetRootComponent());
                    if (PrimitiveComp && PrimitiveComp->GetCollisionEnabled() != ECollisionEnabled::NoCollision)
                    {
                        if (Child->Bounds.Intersect(PrimitiveComp->Bounds.GetBox()))
                        {
                            ChildObstacles.Add(Obstacle);
                        }
                    }
                }
            }
            if (ChildObstacles.Num() > 0)
            {
                BuildNodeRecursive(Child.Get(), ChildObstacles);
            }
        }
    }
}

bool UOctree::QueryNodeRecursive(OctreeNode* CurrentNode, const FVector& Start, const FVector& End, const FBox& SweepBox) const
{
    // まずノード自身のBoundsとSweepBoxが交差しているか
    if (!CurrentNode->Bounds.Intersect(SweepBox))
    {
        return true; // 交差していなければ、このノード内はクリア
    }

    // ノードがブロックされている場合、経路はクリアではない
    if (CurrentNode->bIsBlocked)
    {
        return false;
    }

    // リーフノードではない場合、子ノードを再帰的にチェック
    if (CurrentNode->Children.Num() > 0)
    {
        for (const TUniquePtr<OctreeNode>& Child : CurrentNode->Children)
        {
            if (!QueryNodeRecursive(Child.Get(), Start, End, SweepBox))
            {
                return false; // 子ノードでブロックが見つかった
            }
        }
    }

    return true;
}

void UOctree::UpdateNodeRecursive(OctreeNode* CurrentNode, AActor* Obstacle)
{
    //Todo:処理追加
}
