// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Containers/Array.h"
#include "Math/Box.h"

/**
 * 
 */
class ACHILESS_API OctreeNode
{
public:
	FBox Bounds;//このノードの領域
	int32 Depth;//ツリーのノードの深さ

	TArray<TUniquePtr<OctreeNode>> Children;

	//ノード内のオブジェクト
	TArray<TWeakObjectPtr<AActor>> HitObject;

	//ノード内を通行可能かどうか
	bool bIsBlocked;

	OctreeNode(const FBox& InBounds, int32 InDepth);
	~OctreeNode();

	//このノードを分割する
	void Subdivide();

	//ノード内のオブジェクトを追加する
	void AddHitObject(AActor* Object);

	bool IsHitObjectInNode(const FBox& QueryBox);

	void UpdateNodeStatus(float PawnSize);

	bool ShouldSubdivide(float MaxHitObjectPerNode, int32 MaxDepth);
};
