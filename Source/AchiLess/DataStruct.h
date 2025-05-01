// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataStruct.generated.h"

USTRUCT(BlueprintType)
struct FDataStruct
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly)
	float MaxSpeed;
	UPROPERTY(BlueprintReadOnly)
	float MinSpeed;
	UPROPERTY(BlueprintReadOnly)
	float MaxRotationSpeed;
	UPROPERTY(BlueprintReadOnly)
	float Accelerate;
	UPROPERTY(BlueprintReadOnly)
	float AirFriction;
	UPROPERTY(BlueprintReadOnly)
	float TurnSpeed;
	
};

