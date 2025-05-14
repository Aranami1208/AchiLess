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

	//BP�̃��t�@�����X
	FString WidgetPath = "/Game/UI/BP_SelectAchiLess.BP_SelectAchiLess";

	//�N���X���擾
	TSubclassOf<UUserWidget> WidgetClass = TSoftClassPtr<UUserWidget>(FSoftObjectPath(*WidgetPath)).LoadSynchronous();

	//0�Ԗڂ̃R���g���[���̃|�C���^�擾
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	if (!WidgetClass || !PlayerController)
	{
		UKismetSystemLibrary::PrintString(this, "NoWidgetClass or PlayerController");
		return;
	}

	//CleateWidget����

	UUserWidget* UserWidget = UWidgetBlueprintLibrary::Create(GetWorld(), WidgetClass, PlayerController);

	//UserWidget->AddToViewport(0);
}

void ASelectLevelActor::LoadAllJson()
{
	//���ׂĂ�Json�̃p�X
	TArray<FString> JsonFilePaths;
	FString Directory(FPaths::ProjectContentDir() / "Json");

	//�f�B���N�g���[����t�@�C���p�X���擾
	IFileManager::Get().FindFiles(JsonFilePaths, *Directory, true, false);

	//�f�[�^�}�l�[�W���[
	UADataManager* DataManager = NewObject<UADataManager>();

	//�擾�ł����t�@�C���������J��Ԃ�
	for (const FString& FileName : JsonFilePaths)
	{
		//�f�[�^�ꎟ�ێ��p
		FDataStruct Data;

		//�f�[�^���擾
		DataManager->ReadJsonData(FileName, Data);

		//���X�g�ɒǉ�
		AchiLessParameters.Add(Data);
	}
}

// Called every frame
void ASelectLevelActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

