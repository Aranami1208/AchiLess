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

	LoadAllJson();


}

void ASelectLevelActor::LoadAllJson()
{
	TArray<FString> FilePaths;

	FString Directory(FPaths::ProjectContentDir() + "Json");

	IFileManager::Get().FindFiles(FilePaths, *Directory, true, false);

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

}

// Called every frame
void ASelectLevelActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

