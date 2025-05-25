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

	/// <summary>
	/// 変更待ち状態にする
	/// </summary>
	UFUNCTION(BlueprintCallable)
	void SetIsWaitChange();


	/// <summary>
	/// 変更待ち状態を解除する
	/// </summary>
	void ReleaseIsWaitChange();

	UFUNCTION()
	void LoadCard();

	/// <summary>
	/// 現在設定されているカードを返す
	/// </summary>
	/// <returns>現在のカード</returns>
	FCardData GetCardData();
	
protected:

	UPROPERTY(meta = (BindWidget))
	class UButton* CardButton = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UImage* ReloadImage = nullptr;

	FCardData CardData;


	UPROPERTY(EditAnyWhere,BlueprintReadWrite)
	class UCardDetail* CardDetail;
	

	//カード画像変換中か
	bool bIsWaitChange;
};
 