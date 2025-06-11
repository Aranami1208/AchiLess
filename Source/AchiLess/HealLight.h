// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCardSkill.h"
#include "HealLight.generated.h"

/**
 * 
 */
UCLASS()
class ACHILESS_API UHealLight : public UBaseCardSkill
{
	GENERATED_BODY()

public:
	void ExecuteSkill_Implementation(class ASpaceFighter* Target)override;
	
};
