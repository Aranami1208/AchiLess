#pragma once

#include "CoreMinimal.h"
#include "Math/Box.h"
#include "UObject/NoExportTypes.h" // UObjectを継承するために必要
#include "OctreeNode.generated.h" // ヘッダーのファイル名に合わせて変更

// Octreeの各ノードが持つ情報
UCLASS(BlueprintType) // UObjectを継承したのでUCLASSに変更
class ACHILESS_API UOctreeNode : public UObject // UObjectを継承
{
    GENERATED_BODY()

public:
    // このノードが表す空間の境界ボックス
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Octree")
    FBox Bounds;

    // このノードが障害物を含んでいるかどうか
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Octree")
    bool bContainsObstacle;

    // 子ノードへのポインタ (8つ)
    // UObjectを継承したクラスのポインタなのでUPROPERTYが使用可
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Octree")
    TArray<UOctreeNode*> Children;

    // コンストラクタ (UObjectはデフォルトコンストラクタを持たないため、引数なしコンストラクタを追加)
    UOctreeNode() : bContainsObstacle(false) {}

    // Boundsを設定する初期化関数
    UFUNCTION(BlueprintCallable, Category = "Octree")
    void Initialize(const FBox& InBounds);

    // 子ノードを持つかどうか
    bool HasChildren() const;

    // ノードをクリア (子ノードも再帰的にクリア)
    void ClearChildren();

};