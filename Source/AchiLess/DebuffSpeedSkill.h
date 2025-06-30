// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCardSkill.h"
#include "DebuffSpeedSkill.generated.h"

/**
 * 
 */
UCLASS()
class ACHILESS_API UDebuffSpeedSkill : public UBaseCardSkill
{
	GENERATED_BODY()
	
protected:
	virtual void ExecuteSkill_Implementation(class ASpaceFighter* Target)override;//C++オーバーライドする用

};
