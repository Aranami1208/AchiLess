// Fill out your copyright notice in the Description page of Project Settings.


#include "CardItemWidget.h"
#include "Components/Button.h"
#include "CardDetail.h"
#include "Kismet/KismetSystemLibrary.h"
#include "SelectLevelActor.h"

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
	ReloadImage->SetVisibility(ESlateVisibility::Hidden);

	bIsWaitChange = false;
	
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

	//カード変更待ち状態の時
	if (bIsWaitChange)
	{
		//デッキに同じカードが設定されているときは入れ替える
		TArray<UCardItemWidget*> Decks = CardDetail->SelectLevelActor->DeckCards;

		int32 Index = 0;

		//重複したカードのインデックスを保存する変数
		int32 TempIndex = -1;

		//重複時交換用カード
		FCardData TempCard;

		for (UCardItemWidget* Card : Decks)
		{
			FCardData Data = Card->GetCardData();

			//カードIDが一致している場合インデックスを保存
			if (Data.CardID == CardDetail->GetNextCard().CardID)
			{
				TempIndex = Index;
				TempCard = CardData;
			}

			//インデックス加算
			Index++;
		}

		//カードの情報を書き換える
		CardData = CardDetail->GetNextCard();
		LoadCard();

		CardDetail->SelectLevelActor->ReleaseReload();

		//カードに重複がある場合は入れ替え
		if (TempIndex != -1)
		{
			CardDetail->SelectLevelActor->DeckCards[TempIndex]->CardData = TempCard;
			CardDetail->SelectLevelActor->DeckCards[TempIndex]->LoadCard();
		}

		return;
	}

	CardDetail->ShowCardDetail(CardData);
}

void UCardItemWidget::SetIsWaitChange()
{

	if (!ReloadImage)return;
	

	//リロード画像を描画する
	ReloadImage->SetVisibility(ESlateVisibility::Visible);
	bIsWaitChange = true;
}

void UCardItemWidget::ReleaseIsWaitChange()
{
	if (!ReloadImage)return;
	//リロード画像を非表示にする
	ReloadImage->SetVisibility(ESlateVisibility::Hidden);
	bIsWaitChange = false;
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

FCardData UCardItemWidget::GetCardData()
{
	return CardData;
}

