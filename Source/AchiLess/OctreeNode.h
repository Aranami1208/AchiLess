#pragma once

#include "CoreMinimal.h"
#include "Math/Box.h"
#include "OctreeNode.generated.h" // ヘッダーのファイル名に合わせて変更

// Octreeの各ノードが持つ情報
USTRUCT(BlueprintType) 
struct ACHILESS_API FOctreeNode 
{
    GENERATED_BODY()

    // このノードが表す空間の境界ボックス
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Octree")
    FBox Bounds;

    // このノードが障害物を含んでいるかどうか
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Octree")
    bool bContainsObstacle;

    // 子ノードへのインデックス (8つ)
   // AllNodes配列内のインデックスを保持する
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Octree")
    TArray<int32> ChildrenIndices; // ChildrenをChildrenIndicesに変更

    

    // コンストラクタ
    FOctreeNode() : bContainsObstacle(false)
    {
        ChildrenIndices.SetNum(8); // 8つの子ノード分のスペースを確保 (初期値は0)
        for (int32 i = 0; i < 8; ++i)
        {
            ChildrenIndices[i] = INDEX_NONE; // 初期値として無効なインデックスを設定
        }
    }

    // Boundsを設定するコンストラクタ
    FOctreeNode(const FBox& InBounds) : Bounds(InBounds), bContainsObstacle(false)
    {
        ChildrenIndices.SetNum(8);
        for (int32 i = 0; i < 8; ++i)
        {
            ChildrenIndices[i] = INDEX_NONE;
        }
    }

    bool operator==(const FOctreeNode& Other) const
    {
        // Bounds が同じであれば等価と見なす
        // 必要に応じて他のメンバー変数も比較対象に含めることができます
        return Bounds == Other.Bounds &&
            bContainsObstacle == Other.bContainsObstacle &&
            ChildrenIndices == Other.ChildrenIndices; // ChildrenIndices も比較対象に入れるか検討
    }

    // 子ノードを持つかどうか (インデックスが有効かどうかで判断)
    bool HasChildren() const { return ChildrenIndices[0] != INDEX_NONE; } // 最初のインデックスが有効なら子を持つと判断

    // ノードをクリアする（ChildrenIndicesをリセットするが、実際のノードデータはAllNodesで管理される）
    void ClearChildrenIndices()
    {
        for (int32 i = 0; i < 8; ++i)
        {
            ChildrenIndices[i] = INDEX_NONE;
        }
    }

};