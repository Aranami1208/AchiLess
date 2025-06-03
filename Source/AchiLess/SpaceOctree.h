#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OctreeNode.h"
#include "UObject/NoExportTypes.h" 
#include "SpaceOctree.generated.h"

UCLASS()
class ACHILESS_API ASpaceOctree : public AActor
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    ASpaceOctree();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:
    // 毎フレーム呼ばれる（更新用）
    virtual void Tick(float DeltaTime) override;

    // Octreeのルートノード
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Octree")
    UOctreeNode* RootNode;

    // Octreeの初期サイズ
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Octree")
    FVector Extent = FVector(50000.0f); // 広大な宇宙空間をカバーできるよう大きな値に設定

    // Octreeの最小ノードサイズ (分割を止める閾値)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Octree")
    float MinNodeSize = 1000.0f; // 隕石のサイズや戦闘機のサイズに合わせて調整

    // Octreeを初期化する関数
    UFUNCTION(BlueprintCallable, Category = "Octree")
    void InitializeOctree(const FVector& CenterLocation, const FVector& InExtent);

    // 障害物をOctreeに登録する関数
    UFUNCTION(BlueprintCallable, Category = "Octree")
    void AddObstacle(const FBox& ObstacleBounds);

    // 指定された位置が障害物を含んでいるか判定する関数
    UFUNCTION(BlueprintCallable, Category = "Octree")
    bool IsLocationBlocked(const FVector& Location) const;

    // 指定された境界ボックスが障害物を含んでいるか判定する関数
    UFUNCTION(BlueprintCallable, Category = "Octree")
    bool IsBoxBlocked(const FBox& Box) const;

protected:
    // 再帰的にOctreeノードを初期化するヘルパー関数
    void BuildOctreeNode(UOctreeNode* Node);

    // 再帰的に障害物をOctreeに登録するヘルパー関数
    void AddObstacleToNode(UOctreeNode* Node, const FBox& ObstacleBounds);

    // 再帰的に指定された位置がブロックされているか判定するヘルパー関数
    bool IsLocationBlockedInNode(UOctreeNode* Node, const FVector& Location) const;

    // 再帰的に指定された境界ボックスがブロックされているか判定するヘルパー関数
    bool IsBoxBlockedInNode(UOctreeNode* Node, const FBox& Box) const;

    // デバッグ表示用の描画関数 (オプション)
    void DrawDebugOctreeNode(const UOctreeNode* Node, const FColor& Color) const;
};
