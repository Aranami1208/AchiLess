// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterData.h"
#include "CardItemWidget.h"
#include "Kismet/GameplayStatics.h"
#include "MySaveGame.h"

void UCharacterData::Initialize()
{
	//データをロードする処理　
	saveData = SyncLoad();

	//もうすでにセーブデータをセットされている場合はスキップ
	if (saveData)return;

	UKismetSystemLibrary::PrintString(this, "NewCreateSaveData");
	//データがない場合はデータを新規作成　
	saveData = Cast<UMySaveGame>(UGameplayStatics::CreateSaveGameObject(UMySaveGame::StaticClass()));
}

void UCharacterData::Deinitialize()
{
}

bool UCharacterData::AsyncSave()
{
	//セーブ失敗
	if (!saveData)
	{
		return false;
	}


	//セーブ完了で呼び出す関数を設定　
	FAsyncSaveGameToSlotDelegate delegate = FAsyncSaveGameToSlotDelegate::CreateUObject(this, &UCharacterData::SaveCompleted);

	UGameplayStatics::AsyncSaveGameToSlot(saveData, SlotNameGameData, 0, delegate);

	return true;
}

UMySaveGame* UCharacterData::SyncLoad()
{
	//データが存在するかチェック
	if(!UGameplayStatics::DoesSaveGameExist(SlotNameGameData,0)) return nullptr;

	//スロットネームとプレイヤー番号からセーブデータをロードする
	UMySaveGame* SaveData = Cast<UMySaveGame>(UGameplayStatics::LoadGameFromSlot(SlotNameGameData, 0));

	
	//データを読み込めた場合はデータを返し、読めなかったらnullptrを返す　
	if (SaveData)return SaveData;
	else return nullptr;
}

void UCharacterData::SetParameter(FDataStruct CharaParam)
{
	Parameter = CharaParam;
}

FDataStruct UCharacterData::GetParameter()
{
	return Parameter;
}

void UCharacterData::SetDeckFromData(TArray<UCardItemWidget*> InData)
{
	for (UCardItemWidget* Card : InData)
	{
		Deck.Add(Card->GetCardData());
	}
}

FCardData UCharacterData::GetDeck(int32 Index)
{
	return Deck[Index];
}
TArray<FCardData> UCharacterData::GetDeckAll()
{
	return Deck;
}

bool UCharacterData::UpdateHighScore(int32 Score)
{
	if (!saveData)
	{
		UKismetSystemLibrary::PrintString(this, "SaveDataNotFound");

		return false;
	}
	if (Score > saveData->HighScore)
	{
		saveData->HighScore = Score;
		return true;
	}
	return false;
}

void UCharacterData::SaveCompleted(const FString& SlotName, const int32 UserIndex, bool bSuccess)
{

	UE_LOG(LogTemp, Log, TEXT("UMyGameInstanceSubsystem::SaveCompleted() : [%s][%d][%d]"), *SlotName, UserIndex, bSuccess);

	if (bSuccess)
	{
		UE_LOG(LogTemp, Log, TEXT("success : AsyncSaveGameToSlot()"));
		UKismetSystemLibrary::PrintString(this, "Saved Success");
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("failed : AsyncSaveGameToSlot()"));
		UKismetSystemLibrary::PrintString(this, "Saved Faild");
	}
}
