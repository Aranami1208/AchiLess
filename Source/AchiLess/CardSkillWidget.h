// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CardData.h"
#include "CardItemWidget.h"
#include "CardSkillWidget.generated.h"


UCLASS()
class ACHILESS_API UCardSkillWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void InitCard(FCardData InCard);

	UFUNCTION(BlueprintCallable)
	void StartCoolDown();

	UFUNCTION(BlueprintCallable)
	void EndCoolDown();

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	float percent;

	UPROPERTY(meta = (BindWidget))
	class UImage* CoolTimeGauge;
		
protected:
	//カードデータ
	FCardData Data;

	UPROPERTY(meta = (BindWidget))
	class UImage* Image;
	
	

};
