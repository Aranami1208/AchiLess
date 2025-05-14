// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "HUD_SelectLevel.generated.h"

/**
 * 
 */
UCLASS()
class ACHILESS_API AHUD_SelectLevel : public AHUD
{
	GENERATED_BODY()
	
protected:

	virtual void BeginPlay()override;
};
