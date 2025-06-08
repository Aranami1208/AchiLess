#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h" 
#include "SpaceOctree.h"
#include "AStarNode.h"
#include "PathfindingSubsystem.generated.h"

DECLARE_DYNAMIC_DELEGATE_OneParam(FOnPathfindingComplete, const TArray<FVector>&, Path);

//オクツリー準備完了イベント用のデリゲート
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnOctreeReady);

UCLASS()
class ACHILESS_API UPathfindingSubsystem : public UWorldSubsystem // UWorldSubsystem を継承
{
    GENERATED_BODY()

public:
    // Initialize this subsystem.
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;

    // Deinitialize this subsystem.
    virtual void Deinitialize() override;

    void SetOctreeReference(ASpaceOctree* Ref);

    // Octreeへの参照
    UPROPERTY()
    ASpaceOctree* SpaceOctree;

    // 経路探索を実行する関数
    UFUNCTION(BlueprintCallable, Category = "Pathfinding")
    TArray<FVector> FindPath(const FVector& StartLocation, const FVector& EndLocation);

    UFUNCTION(BlueprintCallable, Category = "Pathfinding")
    void FindPathAsync(const FVector& StartLocation, const FVector& EndLocation, const FOnPathfindingComplete& OnCompleteCallback);

    //処理中かどうか
    bool bIsPathFinding = false;

    //準備完了でブロードキャストされるいべんと
    FOnOctreeReady OnOctreeReady;

    bool IsOctreeReady();
protected:
  

    // A*アルゴリズムのヒューリスティック関数 (推定コスト)
    float CalculateHCost(const FVector& FromLocation, const FVector& ToLocation) const;

    // Octreeノードの隣接ノードを取得する関数
    TArray<int32> GetNeighboringOctreeNodes(int32 CurrentOctreeNodeIndex) const;

    // 経路を再構築する関数
    TArray<FVector> ReconstructPath(int32 EndAStarNodeIndex, TArray<FAStarNode>& InAStarNodes) const;

private:
    bool bIsOctreeInitialized = false;

};
