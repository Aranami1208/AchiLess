// Fill out your copyright notice in the Description page of Project Settings.


#include "Octree.h"
#include "OctreeNode.h"

UOctree::UOctree()
	:HitObjectPerNode(5.0f),MaxOctreeDepth(8),PawnSize(1800.0f)
{
}



void UOctree::BuildOctree(const FBox& WorldBounds, const TArray<AActor*>& AllObstacles)
{
}

void UOctree::ClearOctree()
{
}

void UOctree::UpdateDynamicObstacles(const TArray<AActor*>& DynamicObstacles)
{
}

bool UOctree::IsPathClear(const FVector& StartLocation, const FVector& EndLocation) const
{
	return false;
}

void UOctree::BuildNodeRecursive(OctreeNode* CurrentNode, const TArray<AActor*>& ObstaclesInNode)
{
}

bool UOctree::QueryNodeRecursive(OctreeNode* CurrentNode, const FVector& Start, const FVector& End, const FBox& SweepBox) const
{
	return false;
}

void UOctree::UpdateNodeRecursive(OctreeNode* CurrentNode, AActor* Obstacle)
{
}
