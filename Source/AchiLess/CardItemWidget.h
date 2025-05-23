// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CardData.h"
#include "CardItemWidget.generated.h"


  

UCLASS()
class ACHILESS_API UCardItemWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void InitCard(const FCardData& InCard);

	UFUNCTION()
	void OnCardClicked();

	UFUNCTION(BlueprintCallable)
	void SetIsWaitChange(const FCardData& InCard);

	UFUNCTION()
	void LoadCard();
	
protected:

	UPROPERTY(meta = (BindWidget))
	class UButton* CardButton = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UImage* ReloadImage = nullptr;

	FCardData CardData;

	FCardData NextCard;

	UPROPERTY(EditAnyWhere,BlueprintReadWrite)
	class UCardDetail* CardDetail;


	//ÉJÅ[ÉhâÊëúïœä∑íÜÇ©
	bool bIsWaitChange;
};
 