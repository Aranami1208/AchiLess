// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "DataStruct.h"
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
	UFUNCTION(BlueprintCallable)
	void SetParameter(FDataStruct CharaParam);
	FDataStruct GetParameter();

private:

	//プレイヤーのパラメータ情報を保持
	FDataStruct Parameter;
	
};
