// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCardSkill.h"
#include "BuffBeam.generated.h"

/**
 * 
 */
UCLASS()
class ACHILESS_API UBuffBeam : public UBaseCardSkill
{
	GENERATED_BODY()
	
public:
	void ExecuteSkill_Implementation(class ASpaceFighter* Target)override;

	UFUNCTION()
	void RestoreEffect();

	float FireIntervalRate;
	//Œø‰ÊŽžŠÔ
	float EffectTime;

	FTimerHandle RestoreTimer;

	ASpaceFighter* Target;
};
