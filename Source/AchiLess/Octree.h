// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "OctreeNode.h"
#include "Octree.generated.h"

/**
 * 
 */
UCLASS()
class ACHILESS_API UOctree : public UObject
{
	GENERATED_BODY()
	
public:
    TUniquePtr<OctreeNode> RootNode;
    float MaxHitObjectPerNode; // ノードが分割される閾値
    int32 MaxOctreeDepth;      // 8分木の最大深さ
    float PawnSize;         // AIのサイズ (経路の幅を考慮するため)

    UOctree();
    virtual ~UOctree() noexcept;

    // 8分木を構築する
    void BuildOctree(const FBox& WorldBounds, const TArray<AActor*>& AllObstacles);

    // 8分木をクリアする
    void ClearOctree();

    // 動的障害物やプレイヤーの位置に基づいて8分木を更新する
    void UpdateDynamicObstacles(const TArray<AActor*>& DynamicObstacles);

    // 経路探索のために、特定の経路がクリアされているかをクエリする
    // これは主要なクエリ関数
    bool IsPathClear(const FVector& StartLocation, const FVector& EndLocation) const;

private:
    // 再帰的にノードを構築するヘルパー関数
    void BuildNodeRecursive(OctreeNode* CurrentNode, const TArray<AActor*>& ObstaclesInNode);

    // 再帰的にノエリと交差するノードを探索するヘルパー関数
    bool QueryNodeRecursive(OctreeNode* CurrentNode, const FVector& Start, const FVector& End, const FBox& SweepBox) const;

    // 動的な障害物によって影響を受けるノードを更新するヘルパー関数
    void UpdateNodeRecursive(OctreeNode* CurrentNode, AActor* Obstacle);
};
