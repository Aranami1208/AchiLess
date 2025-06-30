// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCardSkill.h"
#include "Class_AchiLess.h"
#include "DebuffBeam.generated.h"


/**
 * 
 */
UCLASS()
class ACHILESS_API UDebuffBeam : public UBaseCardSkill
{
	GENERATED_BODY()
	
protected:
	virtual void ExecuteSkill_Implementation(ASpaceFighter* Target)override;
	void RestoreEffect();
	FTimerHandle TimerHandle;

	AClass_AchiLess* TargetActor;

	float AddInterval;
	float EffectTime;
};
