// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CardData.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "CardDetail.generated.h"

/**
 * 
 */
UCLASS()
class ACHILESS_API UCardDetail : public UUserWidget
{
	GENERATED_BODY()


public:

	void ShowCardDetail(FCardData& InCard);

	UFUNCTION(BlueprintCallable)
	void Confirm();

	UFUNCTION(BlueprintCallable)
	void Close();

private:
	UPROPERTY(meta = (BindWidget))
	class UImage* DetailImage;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* NameText;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* FravorText;
	
	
	FCardData CardData;
	
};
