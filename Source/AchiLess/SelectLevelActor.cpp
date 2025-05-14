// Fill out your copyright notice in the Description page of Project Settings.


#include "SelectLevelActor.h"
#include "HAL/FileManager.h"
#include "Misc/Paths.h"
#include "ADataManager.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Blueprint/WidgetBlueprintLibrary.h"


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

	//BPのリファレンス
	FString WidgetPath = "/Game/UI/BP_SelectAchiLess.BP_SelectAchiLess";

	//クラスを取得
	TSubclassOf<UUserWidget> WidgetClass = TSoftClassPtr<UUserWidget>(FSoftObjectPath(*WidgetPath)).LoadSynchronous();

	//0番目のコントローラのポインタ取得
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	if (!WidgetClass || !PlayerController)
	{
		UKismetSystemLibrary::PrintString(this, "NoWidgetClass or PlayerController");
		return;
	}

	//CleateWidgetする

	UUserWidget* UserWidget = UWidgetBlueprintLibrary::Create(GetWorld(), WidgetClass, PlayerController);

	//UserWidget->AddToViewport(0);
}

void ASelectLevelActor::LoadAllJson()
{
	//すべてのJsonのパス
	TArray<FString> JsonFilePaths;
	FString Directory(FPaths::ProjectContentDir() / "Json");

	//ディレクトリーからファイルパスを取得
	IFileManager::Get().FindFiles(JsonFilePaths, *Directory, true, false);

	//データマネージャー
	UADataManager* DataManager = NewObject<UADataManager>();

	//取得できたファイル分だけ繰り返す
	for (const FString& FileName : JsonFilePaths)
	{
		//データ一次保持用
		FDataStruct Data;

		//データを取得
		DataManager->ReadJsonData(FileName, Data);

		//リストに追加
		AchiLessParameters.Add(Data);
	}
}

// Called every frame
void ASelectLevelActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

