// Fill out your copyright notice in the Description page of Project Settings.


#include "SelectLevelActor.h"
#include "HAL/FileManager.h"
#include "Misc/Paths.h"
#include "ADataManager.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "CardData.h"
#include "CardItemWidget.h"
#include "Components/UniformGridSlot.h"
#include "Components/UniformGridPanel.h"


// Sets default values
ASelectLevelActor::ASelectLevelActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASelectLevelActor::BeginPlay()
{
	Super::BeginPlay();

	//Jsonファイルを一括読み込み
	LoadAllJson();

	//グリッドパネルがない場合は処理しない
	if (!GridPanel)return;

	UKismetSystemLibrary::PrintString(this, "GridPanelLoadOK");

	FString ContextString =TEXT("Loading Card Data Table");
	UDataTable* CardDataTable = LoadObject<UDataTable>(nullptr, TEXT("/Game/Assets/Card/CardDataTable.CardDataTable"));
	TArray<FCardData*> AllCards;

	if (!CardDataTable)return;
	//データテーブルのすべての行を取得
	CardDataTable->GetAllRows<FCardData>(ContextString, AllCards);

	//カードの列数
	const int32 CardColums = 4;
	int32 Index = 0;

	for (FCardData* Card : AllCards)
	{
		UCardItemWidget* CardWidget = CreateWidget<UCardItemWidget>(GetWorld(), CardItemClass);

		CardWidget->InitCard(*Card);

		//表示する行
		int32 Row = Index / CardColums;
		//表示する列
		int32 Col = Index % CardColums;
		
		
		UUniformGridSlot* Slot = GridPanel->AddChildToUniformGrid(CardWidget, Row, Col);
		if (!Slot)
		{
			UKismetSystemLibrary::PrintString(this, "NoSlot");
		}
		UE_DEBUG_BREAK();
		CardWidget->SetVisibility(ESlateVisibility::Visible);
	
		GenaratedWidgets.Add(CardWidget);
		++Index;
	}

	DeckCard1->InitCard(*AllCards[0]);
	DeckCard2->InitCard(*AllCards[1]);
	DeckCard3->InitCard(*AllCards[2]);
	DeckCard4->InitCard(*AllCards[3]);
}

void ASelectLevelActor::LoadAllJson()
{
	TArray<FString> FilePaths;

	FString Directory(FPaths::ProjectContentDir() + "Json/");

	IFileManager::Get().FindFiles(FilePaths, *(Directory + "*.json"), true, false);

	FileNum = FilePaths.Num();

	//UKismetSystemLibrary::PrintString(this, Directory);

	UKismetSystemLibrary::PrintString(this, "Files:" + FString::FromInt(FilePaths.Num()));

	UADataManager* DataManager = NewObject<UADataManager>();

	for (const FString FilePath : FilePaths)
	{
		FDataStruct Data;
		DataManager->ReadJsonData(FilePath, Data);
		AchiLessParams.Add(Data);
	}
}

void ASelectLevelActor::CreateSelectLevelWidget()
{
	FString WidgetPath("/ Game / UI / Layout.Layout");


}

// Called every frame
void ASelectLevelActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASelectLevelActor::AchiLessSelectLeft()
{
	UKismetSystemLibrary::PrintString(this, "push");
	SelectIndex--;
	if (SelectIndex < 0)
	{
		SelectIndex = FileNum - 1;
	}
	UKismetSystemLibrary::PrintString(this, FString::FromInt(SelectIndex));
}

void ASelectLevelActor::AchiLessSelectRight()
{
	UKismetSystemLibrary::PrintString(this, "push");
	SelectIndex++;
	if (SelectIndex >= FileNum)
	{
		SelectIndex = 0;
	}
	UKismetSystemLibrary::PrintString(this, FString::FromInt(SelectIndex));
}

void ASelectLevelActor::SetReload()
{
	
}

