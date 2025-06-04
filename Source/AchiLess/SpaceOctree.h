#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OctreeNode.h" 
#include "SpaceOctree.generated.h"

UCLASS()
class ACHILESS_API ASpaceOctree : public AActor
{
    GENERATED_BODY()

public:
    ASpaceOctree();

protected:
    virtual void BeginPlay() override;

public:
    int32 MaxDepth = 12;

    virtual void Tick(float DeltaTime) override;

    // 全てのオクツリーノードを格納する配列
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Octree")
    TArray<FOctreeNode> AllNodes; // 全てのノードをこの配列で管理

    // Octreeのルートノードのインデックス
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Octree")
    int32 RootNodeIndex; // UOctreeNode*からint32に変更

    // Octreeの初期サイズ
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Octree")
    FVector Extent = FVector(50000.0f);

    // Octreeの最小ノードサイズ (分割を止める閾値)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Octree")
    float MinNodeSize = 1000.0f;

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

    // 特定のインデックスのノードを取得するヘルパー関数
    FOctreeNode* GetNode(int32 NodeIndex);
    const FOctreeNode* GetNode(int32 NodeIndex) const;


    
    FOctreeNode GetOctreeNodeAtLocation(const FVector& Location);

//protected:
    // ノードを8つの子ノードに分割するヘルパー関数
    void SubdivideNode(int32 NodeIndex); // UOctreeNode*からint32に変更

    // 再帰的に障害物をOctreeに登録するヘルパー関数
    void AddObstacleToNode(int32 NodeIndex, const FBox& ObstacleBounds,int32 Depth); // UOctreeNode*からint32に変更

    // 再帰的に指定された位置がブロックされているか判定するヘルパー関数
    bool IsLocationBlockedInNode(int32 NodeIndex, const FVector& Location) const; // UOctreeNode*からint32に変更

    // 再帰的に指定された境界ボックスがブロックされているか判定するヘルパー関数
    bool IsBoxBlockedInNode(int32 NodeIndex, const FBox& Box) const; // UOctreeNode*からint32に変更

    // デバッグ表示用の描画関数 (オプション)
    void DrawDebugOctreeNode(int32 NodeIndex, const FColor& Color) const; // UOctreeNode*からint32に変更
};