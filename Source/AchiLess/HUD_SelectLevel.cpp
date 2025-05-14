// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD_SelectLevel.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

void AHUD_SelectLevel::BeginPlay()
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