#pragma once

#include "CoreMinimal.h"
#include "OctreeNode.h"
#include "AStarNode.generated.h"

UCLASS()
class ACHILESS_API UAStarNode : public UObject
{
    GENERATED_BODY()

public:
    UPROPERTY()
    UOctreeNode* OctreeNode; // このA*ノードが参照するOctreeノード

    UPROPERTY()
    UAStarNode* Parent; // このノードに到達するための親ノード

    UPROPERTY()
    float GCost; // スタート地点からの実際の移動コスト

    UPROPERTY()
    float HCost; // 目標地点までの推定移動コスト (ヒューリスティック)

    UPROPERTY()
    float FCost; // GCost + HCost

    // コンストラクタ
    UAStarNode() : OctreeNode(nullptr), Parent(nullptr), GCost(FLT_MAX), HCost(FLT_MAX), FCost(FLT_MAX) {}

    void Initialize(UOctreeNode* InOctreeNode, UAStarNode* InParent, float InGCost, float InHCost);

    // OctreeNodeの境界の中心を返すヘルパー関数
    FVector GetCenter() const;
};