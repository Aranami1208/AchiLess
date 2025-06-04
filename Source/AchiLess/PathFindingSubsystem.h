#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h" 
#include "SpaceOctree.h"
#include "AStarNode.h"
#include "PathfindingSubsystem.generated.h"

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

protected:
    // A*アルゴリズムの全ノードを格納する配列
   // FAStarNodeはUSTRUCTなので、直接配列に格納できる
    TArray<FAStarNode> AllAStarNodes;

    // A*アルゴリズムのヒューリスティック関数 (推定コスト)
    float CalculateHCost(const FVector& FromLocation, const FVector& ToLocation) const;

    // Octreeノードの隣接ノードを取得する関数
    TArray<int32> GetNeighboringOctreeNodes(int32 CurrentOctreeNodeIndex) const;

    // 経路を再構築する関数
    TArray<FVector> ReconstructPath(int32 EndAStarNodeIndex) const;

   

    // FAStarNodeのインデックスからOctreeNodeのFVector Centerを取得するヘルパー関数
    FVector GetAStarNodeCenter(int32 AStarNodeIndex) const;

    
};
