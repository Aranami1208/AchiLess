#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "PathfindingSubsystem.h" 
#include "MyAIController.generated.h"

UCLASS()
class ACHILESS_API AMyAIController : public AAIController
{
    GENERATED_BODY()

public:
    AMyAIController();

protected:
    virtual void BeginPlay() override;

public:
    // AIの現在のターゲットパス (ブループリントからアクセス可能にするためにUPROPERTYとBlueprintReadOnly)
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI Pathfinding")
    TArray<FVector> CurrentPath;

    // 現在パスのどのポイントに向かっているかを示すインデックス
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI Pathfinding")
    int32 CurrentPathIndex;

    // PathfindingSubsystemへの参照
    UPROPERTY()
    UPathfindingSubsystem* PathfindingSubsystem;

    // ブループリントから経路探索を開始するための関数
    UFUNCTION(BlueprintCallable, Category = "AI Pathfinding")
    void RequestPathToLocation(const FVector& TargetLocation);

    //経路探索完了時に呼ばれる関数
    UFUNCTION()//デリゲートをバインドするために必要
    void OnPathFindingCompleted(const TArray<FVector>& Path);

    // AIが次の経路ポイントに移動するためのRotatorを取得する関数
    UFUNCTION(BlueprintCallable, Category = "AI Pathfinding")
    FRotator GetNextPathPointRotation(float DeltaTime, float AcceptanceRadius, UPARAM(ref) float& ToNextPointDistance);

    // パス上の次のポイントに進む
    UFUNCTION(BlueprintCallable, Category = "AI Pathfinding")
    void AdvanceToNextPathPoint();

    // パスが有効かどうか
    UFUNCTION(BlueprintCallable, Category = "AI Pathfinding")
    bool HasValidPath() const;

    // 現在のターゲットパス上のポイントを取得
    UFUNCTION(BlueprintCallable, Category = "AI Pathfinding")
    FVector GetCurrentTargetPathPoint() const;
};