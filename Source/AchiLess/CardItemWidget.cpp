// Fill out your copyright notice in the Description page of Project Settings.


#include "CardItemWidget.h"
#include "Components/Button.h"
#include "CardDetail.h"
#include "Kismet/KismetSystemLibrary.h"


void UCardItemWidget::InitCard(const FCardData& InCard)
{
	CardData = InCard;

	
	if (!ReloadImage)
	{
		UKismetSystemLibrary::PrintString(this, "NoReloadImage");
		return;
	}

	if (!CardButton)
	{
		UKismetSystemLibrary::PrintString(this, "NoButton");
		return;
	}

	//最初はリロード画像は非表示
	//ReloadImage->SetVisibility(ESlateVisibility::Hidden);


	
	FButtonStyle NewStyle = CardButton->WidgetStyle;

	float ViewCardScale = 0.6;
	//通常状態の画像設定 
	NewStyle.Normal.SetResourceObject(CardData.CardImage.LoadSynchronous());
	NewStyle.Normal.ImageSize = FVector2D(CardData.CardImage->GetSizeX() * ViewCardScale, CardData.CardImage->GetSizeY() * ViewCardScale);
	NewStyle.Normal.TintColor = FLinearColor(1.0, 1.0f, 1.0f, 1.0f);
	//カーソルを合わせたときの画像設定
	NewStyle.Hovered.SetResourceObject(CardData.CardImage.LoadSynchronous());
	NewStyle.Hovered.ImageSize = FVector2D(CardData.CardImage->GetSizeX() * ViewCardScale, CardData.CardImage->GetSizeY() * ViewCardScale);
	NewStyle.Hovered.TintColor = FLinearColor(0.7f, 0.7f, 0.7f, 1.0f);
	//ボタンを押したときの設定
	NewStyle.Pressed.SetResourceObject(CardData.CardImage.LoadSynchronous());
	NewStyle.Pressed.ImageSize = FVector2D(CardData.CardImage->GetSizeX() * ViewCardScale, CardData.CardImage->GetSizeY() * ViewCardScale);
	NewStyle.Pressed.TintColor = FLinearColor(0.5f, 0.5f, 0.5f, 1.0f);
	CardButton->SetStyle(NewStyle);


	//AddDynamicだとBlueprintと連携が可能(Add出は不可)
	CardButton->OnClicked.AddDynamic(this, &UCardItemWidget::OnCardClicked);
	UKismetSystemLibrary::PrintString(this, "Create Card Item WIdget");
}

void UCardItemWidget::OnCardClicked()
{
	UKismetSystemLibrary::PrintString(this, "OnCardClicked");

	if (!CardDetail)
	{
		UKismetSystemLibrary::PrintString(this, "CardDetailNotFound");
		return;
	}

	CardDetail->ShowCardDetail(CardData);
}

void UCardItemWidget::SetIsWaitChange(const FCardData& InCard)
{

	if (!ReloadImage)return;
	//変更先のカードを設定する
	NextCard = InCard;

	//リロード画像を描画する
	ReloadImage->SetVisibility(ESlateVisibility::Visible);
	bIsWaitChange = true;
}

void UCardItemWidget::LoadCard()
{
	if (!CardDetail)
	{
		UKismetSystemLibrary::PrintString(this, "CardDetailNotFound");
		return;
	}

	FButtonStyle NewStyle = CardButton->WidgetStyle;

	float ViewCardScale = 0.6;
	//通常状態の画像設定 
	NewStyle.Normal.SetResourceObject(CardData.CardImage.LoadSynchronous());
	NewStyle.Normal.ImageSize = FVector2D(CardData.CardImage->GetSizeX() * ViewCardScale, CardData.CardImage->GetSizeY() * ViewCardScale);
	NewStyle.Normal.TintColor = FLinearColor(1.0, 1.0f, 1.0f, 1.0f);
	//カーソルを合わせたときの画像設定
	NewStyle.Hovered.SetResourceObject(CardData.CardImage.LoadSynchronous());
	NewStyle.Hovered.ImageSize = FVector2D(CardData.CardImage->GetSizeX() * ViewCardScale, CardData.CardImage->GetSizeY() * ViewCardScale);
	NewStyle.Hovered.TintColor = FLinearColor(0.7f, 0.7f, 0.7f, 1.0f);
	//ボタンを押したときの設定
	NewStyle.Pressed.SetResourceObject(CardData.CardImage.LoadSynchronous());
	NewStyle.Pressed.ImageSize = FVector2D(CardData.CardImage->GetSizeX() * ViewCardScale, CardData.CardImage->GetSizeY() * ViewCardScale);
	NewStyle.Pressed.TintColor = FLinearColor(0.5f, 0.5f, 0.5f, 1.0f);
	CardButton->SetStyle(NewStyle);

}
