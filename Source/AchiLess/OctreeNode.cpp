// Fill out your copyright notice in the Description page of Project Settings.


#include "OctreeNode.h"
#include "GameFramework/Actor.h"
#include "Components/PrimitiveComponent.h"
#include "Math/Box.h"

OctreeNode::OctreeNode(const FBox& InBounds, int32 InDepth)
	:Bounds(InBounds),Depth(InDepth),bIsBlocked(false)
{
	Children.Reserve(8);
}

OctreeNode::~OctreeNode()
{
}

void OctreeNode::Subdivide()
{
	// 既に分割されている場合は何もしない
	if (Children.Num() > 0)
	{
		return;
	}

	FVector Center = Bounds.GetCenter();
	FVector Extent = Bounds.GetExtent(); // Boundsの半分のサイズ

	Children.Add(MakeUnique<OctreeNode>(FBox(Center - FVector(Extent.X, Extent.Y, Extent.Z), Center), Depth + 1)); // -X -Y -Z
	Children.Add(MakeUnique<OctreeNode>(FBox(FVector(Center.X - Extent.X, Center.Y, Center.Z - Extent.Z), FVector(Center.X, Center.Y + Extent.Y, Center.Z)), Depth + 1)); // -X +Y -Z (これは一例)
	Children.Add(MakeUnique<OctreeNode>(FBox(FVector(Center.X, Center.Y - Extent.Y, Center.Z - Extent.Z), FVector(Center.X + Extent.X, Center.Y, Center.Z)), Depth + 1)); // +X -Y -Z
	Children.Add(MakeUnique<OctreeNode>(FBox(FVector(Center.X, Center.Y, Center.Z - Extent.Z), FVector(Center.X + Extent.X, Center.Y + Extent.Y, Center.Z)), Depth + 1)); // +X +Y -Z
	Children.Add(MakeUnique<OctreeNode>(FBox(FVector(Center.X - Extent.X, Center.Y - Extent.Y, Center.Z), FVector(Center.X, Center.Y, Center.Z + Extent.Z)), Depth + 1)); // -X -Y +Z
	Children.Add(MakeUnique<OctreeNode>(FBox(FVector(Center.X - Extent.X, Center.Y, Center.Z), FVector(Center.X, Center.Y + Extent.Y, Center.Z + Extent.Z)), Depth + 1)); // -X +Y +Z
	Children.Add(MakeUnique<OctreeNode>(FBox(FVector(Center.X, Center.Y - Extent.Y, Center.Z), FVector(Center.X + Extent.X, Center.Y, Center.Z + Extent.Z)), Depth + 1)); // +X -Y +Z
	Children.Add(MakeUnique<OctreeNode>(FBox(Center, Center + FVector(Extent.X, Extent.Y, Extent.Z)), Depth + 1)); // +X +Y +Z

}

void OctreeNode::AddHitObject(AActor* Object)
{
	if (Object && Object->IsValidLowLevel())
	{
		HitObject.Add(Object);
	}
}

bool OctreeNode::IsHitObjectInNode(const FBox& QueryBox)
{
	// このノードのBoundsとクエリボックスがそもそも交差していなければ、障害物も交差しない
	if (!Bounds.Intersect(QueryBox))
	{
		return false;
	}

	// リーフノードの場合、含まれる障害物のBoundsとクエリボックスをチェック
	// 厳密にはここではなく、UOctree::QueryNodeRecursiveで最終的な衝突判定を行うことが多い
	// bIsBlockedがtrueであれば、このノード自体が障害物として扱われるため、この関数は不要な場合もある
	for (TWeakObjectPtr<AActor> Object : HitObject)
	{
		if (Object.IsValid())
		{
			UPrimitiveComponent* PrimitiveComp = Cast<UPrimitiveComponent>(Object->GetRootComponent());
			if (PrimitiveComp && PrimitiveComp->GetCollisionEnabled() != ECollisionEnabled::NoCollision)
			{
				if (QueryBox.Intersect(PrimitiveComp->Bounds.GetBox()))
				{
					return true;
				}
			}
		}
	}
	return false;
}

void OctreeNode::UpdateNodeStatus(float PawnSize)
{
	//ノードのサイズがPawnのサイズより小さい時は通れないのでbIsBlockedにする
	float MinDimension = FMath::Min(Bounds.GetExtent().X, FMath::Min(Bounds.GetExtent().Y, Bounds.GetExtent().Z)) * 2.0f; // *2.0fで全体の幅に戻す
	if (MinDimension < PawnSize)
	{
		bIsBlocked = true; // Pawnが通過するには狭すぎる
		return;
	}

	//ノード内にオブジェクトがある場合はtrue
	bIsBlocked = HitObject.Num() > 0;



}

bool OctreeNode::ShouldSubdivide(float MaxHitObjectPerNode, int32 MaxDepth)
{
	// 最大深さに達していない かつ ノード内の障害物数が閾値を超えている
	return Depth < MaxDepth && HitObject.Num() > MaxHitObjectPerNode;
}
