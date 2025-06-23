// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "DataStruct.h"
#include "CardData.h"
#include "PathFindingSubsystem.h"
#include "MySaveGame.h"
#include "CharacterData.generated.h"

/**
 * 
 */
UCLASS()
class ACHILESS_API UCharacterData : public UGameInstance
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	virtual void Initialize();

	UFUNCTION(BlueprintCallable)
	virtual void Deinitialize();

	UFUNCTION(BlueprintCallable)
	bool AsyncSave();

	UFUNCTION(BlueprintCallable)
	UMySaveGame* SyncLoad();

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

	/// <summary>
	/// HighScore更新
	/// </summary>
	/// <param name="Score">今回のスコア</param>
	/// <returns>HighScoreを更新された場合はtrue、更新されなかった場合はfalse</returns>
	UFUNCTION(BlueprintCallable, Category = "CharacterData")
	bool UpdateHighScore(int32 Score);

	//敵AchiLessの名前
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString EnemyAchiLessName;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FCardData EmptyData;

	UPROPERTY()
	TObjectPtr<UTexture2D> EmptyImage;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FName CurrentLevel = "None";

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName NextLevel = "None";

	UFUNCTION(BlueprintCallable, Category = "CharacterData")
	const class UMySaveGame* GetSaveData() { return saveData; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MyScore = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 KillPoint = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 DestructPoint = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 EvationPoint = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 EnemyScore = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool UpdateHigh = false;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TArray<FCardData> DropCardData;
private:

	//プレイヤーのパラメータ情報を保持
	FDataStruct Parameter;

	//プレイヤーのデッキに入っているカード
	TArray<FCardData> Deck;

	const FString SlotNameGameData = FString("SaveData");


	//セーブデータ
	UPROPERTY()
	class UMySaveGame* saveData = nullptr;

	UFUNCTION()
	void SaveCompleted(const FString& SlotName, const int32 UserIndex, bool bSuccess);
	
};
