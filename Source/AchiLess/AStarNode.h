#pragma once

#include "CoreMinimal.h"

#include "AStarNode.generated.h"

USTRUCT(BlueprintType) 
struct ACHILESS_API FAStarNode // U AStarNodeから F AStarNode に変更
{
    GENERATED_BODY()

public:
    UPROPERTY()
    int32 OctreeNodeIndex; // このA*ノードが参照するOctreeノード

    UPROPERTY()
    int32 ParentIndex; // このノードに到達するための親ノード

    UPROPERTY()
    float GCost; // スタート地点からの実際の移動コスト

    UPROPERTY()
    float HCost; // 目標地点までの推定移動コスト (ヒューリスティック)

    UPROPERTY()
    float FCost; // GCost + HCost

    // コンストラクタ
    FAStarNode()
        : OctreeNodeIndex(INDEX_NONE)
        , ParentIndex(INDEX_NONE) // 親ノードのインデックスを無効値で初期化
        , GCost(FLT_MAX)
        , HCost(FLT_MAX)
        , FCost(FLT_MAX)
    {
    }

    void Initialize(int32 InOctreeNodeIndex, int32 InParentIndex, float InGCost, float InHCost)
    {
        OctreeNodeIndex = InOctreeNodeIndex;
        ParentIndex = InParentIndex;
        GCost = InGCost;
        HCost = InHCost;
        FCost = GCost + HCost;
    }

    
};