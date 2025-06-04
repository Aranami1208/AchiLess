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
	UFUNCTION(BlueprintCallable)
	static class ASpaceFighter* CheckOnTarget(UWorld* WorldContext,class ASpaceFighter* Owner,FVector Location, FVector Direction, float Radius, float MaxDistance,float FOV);
	
	UFUNCTION(BlueprintCallable)
	static FRotator CalcToPreTargetRotation(UWorld* WorldContext, class ASpaceFighter* Owner, float TargetSpeed, FVector TargetLocation, FVector TargetVelocity);
};
