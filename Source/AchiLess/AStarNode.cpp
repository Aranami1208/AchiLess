// Fill out your copyright notice in the Description page of Project Settings.


#include "AStarNode.h"

void UAStarNode::Initialize(UOctreeNode* InOctreeNode, UAStarNode* InParent, float InGCost, float InHCost)
{
    OctreeNode = InOctreeNode;
    Parent = InParent;
    GCost = InGCost;
    HCost = InHCost;
    FCost = GCost + HCost;
}

FVector UAStarNode::GetCenter() const
{
	return OctreeNode ? OctreeNode->Bounds.GetCenter() : FVector::ZeroVector;
}
