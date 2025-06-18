// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "DataStruct.h"
#include "CardData.h"
#include "PathFindingSubsystem.h"
#include "CharacterData.generated.h"

/**
 * 
 */
UCLASS()
class ACHILESS_API UCharacterData : public UGameInstance
{
	GENERATED_BODY()

public:
	//parameterをセットする
	UFUNCTION(BlueprintCallable, Category = "CharacterData")
	void SetParameter(FDataStruct CharaParam);
	
	FDataStruct GetParameter();

	UFUNCTION(BlueprintCallable, Category = "CharacterData")
	void SetDeckFromData(TArray<class UCardItemWidget*> InData);

	UFUNCTION(BlueprintCallable, Category = "CharacterData")
	FCardData GetDeck(int32 Index);
	UFUNCTION(BlueprintCallable, Category = "CharacterData")
	TArray<FCardData> GetDeckAll();

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FCardData EmptyData;

	UPROPERTY()
	TObjectPtr<UTexture2D> EmptyImage;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FName CurrentLevel = "None";

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName NextLevel = "None";

private:

	//プレイヤーのパラメータ情報を保持
	FDataStruct Parameter;

	//プレイヤーのデッキに入っているカード
	TArray<FCardData> Deck;
	
};
