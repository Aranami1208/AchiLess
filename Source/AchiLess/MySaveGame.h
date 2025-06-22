// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "MySaveGame.generated.h"

/**
 * 
 */
UCLASS()
class ACHILESS_API UMySaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<int32> UnlockCardID;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int32 HighScore = 0;
};
