// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD_SelectLevel.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

void AHUD_SelectLevel::BeginPlay()
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