// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CardData.h"
#include "CardItemWidget.generated.h"

/**
 * 
 */
UCLASS()
class ACHILESS_API UCardItemWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void InitCard(const FCardData& InCard);
	
protected:

	UPROPERTY(meta = (BindWidget))
	class UButton* CardButton;

	FCardData CardData;
};
