#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OctreeNode.h" // OctreeNode.hをインクルード
#include "SpaceOctree.generated.h"

// 前方宣言
class UPrimitiveComponent;

UCLASS()
class ACHILESS_API ASpaceOctree : public AActor
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    ASpaceOctree();

    // Octreeの初期化
    UFUNCTION(BlueprintCallable, Category = "Octree")
    void InitializeOctree(const FVector& CenterLocation, const FVector& InExtent);

    // 特定の位置にあるOctreeノードを取得
    UFUNCTION(BlueprintCallable, Category = "Octree")
    FOctreeNode GetOctreeNodeAtLocation(const FVector& Location);

    // 障害物をOctreeに追加 (AActor* を引数に取るように変更)
    UFUNCTION(BlueprintCallable, Category = "Octree")
    void AddObstacle(AActor* ObstacleActor);

    // 指定された位置がブロックされているか確認
    UFUNCTION(BlueprintCallable, Category = "Octree")
    bool IsLocationBlocked(const FVector& Location) const;

    // 指定されたボックスがブロックされているか確認
    UFUNCTION(BlueprintCallable, Category = "Octree")
    bool IsBoxBlocked(const FBox& Box) const;

    // Octreeのルートノードインデックス
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Octree")
    int32 RootNodeIndex;

    // Octreeの全ノードを格納する配列
    UPROPERTY() // Blueprintからは直接アクセスさせない想定
        TArray<FOctreeNode> AllNodes;

    // Octreeの空間的な広がり (中心からの各軸方向の距離)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Octree")
    FVector Extent;

    // ノードを分割する際の最小サイズ
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Octree", meta = (ClampMin = "1.0"))
    float MinNodeSize = 50000.0f;

    // タグ付きオブジェクトを処理する際の最小ノードサイズ (より細かく分割するため)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Octree", meta = (ClampMin = "1.0"))
    float MinNodeSizeForTaggedObject = 2000.0f;

    // 詳細なコリジョン判定を行うオブジェクトに付けるタグ
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Octree")
    FName SpecialObstacleTag;

    // デバッグ描画用
    UFUNCTION(BlueprintCallable, Category = "Octree|Debug")
    void DrawDebugOctreeNode(int32 NodeIndex, const FColor& Color) const;


    // ヘルパー関数 (PathfindingSubsystem からもアクセスできるように public にする)
    FOctreeNode* GetNode(int32 NodeIndex);
    const FOctreeNode* GetNode(int32 NodeIndex) const;


protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

private:
    // ノードを8つの子ノードに分割
    void SubdivideNode(int32 NodeIndex);

    // 従来のバウンディングボックスベースで障害物をノードに追加する内部関数
    void AddBoundingBoxObstacle(const FBox& ObstacleBounds);
    // AddObstacleToNode は AddBoundingBoxObstacle から呼び出される
    void AddObstacleToNode(int32 NodeIndex, const FBox& ObstacleBounds, int32 Depth);

    // タグ付きオブジェクトのメッシュベースで障害物を追加する関数
    void AddTaggedObstacle(AActor* TaggedActor);
    // タグ付きオブジェクトを処理するための再帰関数
    void AddObstacleToNodeForTaggedObject(int32 NodeIndex, AActor* TaggedActor, UPrimitiveComponent* CollisionComponent, int32 Depth);


    // 指定された位置が特定のノード内でブロックされているか再帰的に確認
    bool IsLocationBlockedInNode(int32 NodeIndex, const FVector& Location) const;
    // 指定されたボックスが特定のノード内でブロックされているか再帰的に確認
    bool IsBoxBlockedInNode(int32 NodeIndex, const FBox& Box) const;

    // デバッグ描画用の再帰関数
    //void DrawDebugOctree(int32 NodeIndex, const FColor& Color) const;
};