// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataStruct.generated.h"

USTRUCT(BlueprintType)
struct FDataStruct
{
	GENERATED_BODY()

public: 

	//�ō����x
	UPROPERTY(BlueprintReadOnly)
	float MaxSpeed;

	//�Œᑬ�x
	UPROPERTY(BlueprintReadOnly)
	float MinSpeed;

	//�s�b�`�̉�]���x
	UPROPERTY(BlueprintReadOnly)
	float MaxRotationSpeed;

	//�u�[�X�g�e��
	UPROPERTY(BlueprintReadOnly)
	float MaxBoost;

	//�����x
	UPROPERTY(BlueprintReadOnly)
	float Accelerate;

	//��C��R
	UPROPERTY(BlueprintReadOnly)
	float AirFriction;

	//���[���̉�]���x

	UPROPERTY(BlueprintReadOnly)
	float TurnSpeed;

	//���b�V���t�@�C����
	UPROPERTY(BlueprintReadOnly)
	FString MeshFileName;
	
};

