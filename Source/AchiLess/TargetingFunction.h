// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "TargetingFunction.generated.h"

/**
 * 
 */
UCLASS()
class ACHILESS_API UTargetingFunction : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	/// <summary>
	/// ロックオンしているかどうかチェックする
	/// </summary>
	/// <param name="WorldContext">ワールドコンテキスト</param>
	/// <param name="Owner">オーナーのポインタ</param>
	/// <param name="Location">オーナーの位置</param>
	/// <param name="Direction">オーナーの前方方向</param>
	/// <param name="Radius">判定に使う円の半径</param>
	/// <param name="MaxDistance">判定する最大距離</param>
	/// <param name="FOV">オーナーの視野角</param>
	/// <returns>ロックオンした対象</returns>
	UFUNCTION(BlueprintCallable)
	static class ASpaceFighter* CheckOnTarget(UWorld* WorldContext,class ASpaceFighter* Owner,FVector Location, FVector Direction, float Radius, float MaxDistance,float FOV);
	
	UFUNCTION(BlueprintCallable)
	static FRotator CalcToPreTargetRotation(UWorld* WorldContext, class ASpaceFighter* Owner, float TargetSpeed, FVector TargetLocation, FVector TargetVelocity,FVector TargetAcceleration);
};
