// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataStruct.generated.h"

USTRUCT(BlueprintType)
struct FDataStruct
{
	GENERATED_BODY()

public: 

	//最高速度
	UPROPERTY(BlueprintReadOnly)
	float MaxSpeed;

	//最低速度
	UPROPERTY(BlueprintReadOnly)
	float MinSpeed;

	//ピッチの回転速度
	UPROPERTY(BlueprintReadOnly)
	float MaxRotationSpeed;

	//ブースト容量
	UPROPERTY(BlueprintReadOnly)
	float MaxBoost;

	//加速度
	UPROPERTY(BlueprintReadOnly)
	float Accelerate;

	//空気抵抗
	UPROPERTY(BlueprintReadOnly)
	float AirFriction;

	//ロールの回転速度

	UPROPERTY(BlueprintReadOnly)
	float TurnSpeed;

	//メッシュファイル名
	UPROPERTY(BlueprintReadOnly)
	FString MeshFileName;
	
};

