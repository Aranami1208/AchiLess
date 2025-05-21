// Fill out your copyright notice in the Description page of Project Settings.


#include "CardItemWidget.h"
#include "Components/Button.h"
#include "CardDetail.h"
#include "Kismet/KismetSystemLibrary.h"


void UCardItemWidget::InitCard(const FCardData& InCard)
{
	CardData = InCard;

	if (!CardButton)
	{
		UKismetSystemLibrary::PrintString(this, "NoButton");
		return;
	}
	/*
	FSlateBrush NewBrush;
	NewBrush.SetResourceObject(InCard.CardImage.LoadSynchronous());
	NewBrush.ImageSize = FVector2D(150, 250);
	*/
	//CardButton = NewObject<UButton>(this);
	
	FButtonStyle NewStyle = CardButton->WidgetStyle;

	float ViewCardScale = 0.6;
	//�ʏ��Ԃ̉摜�ݒ� 
	NewStyle.Normal.SetResourceObject(InCard.CardImage.LoadSynchronous());
	NewStyle.Normal.ImageSize = FVector2D(InCard.CardImage->GetSizeX()*ViewCardScale , InCard.CardImage->GetSizeY()*ViewCardScale);
	NewStyle.Normal.TintColor = FLinearColor(1.0, 1.0f, 1.0f, 1.0f);
	//�J�[�\�������킹���Ƃ��̉摜�ݒ�
	NewStyle.Hovered.SetResourceObject(InCard.CardImage.LoadSynchronous());
	NewStyle.Hovered.ImageSize = FVector2D(InCard.CardImage->GetSizeX() * ViewCardScale, InCard.CardImage->GetSizeY() * ViewCardScale);
	NewStyle.Hovered.TintColor = FLinearColor(0.7f, 0.7f, 0.7f, 1.0f);
	//�{�^�����������Ƃ��̐ݒ�
	NewStyle.Pressed.SetResourceObject(InCard.CardImage.LoadSynchronous());
	NewStyle.Pressed.ImageSize = FVector2D(InCard.CardImage->GetSizeX() * ViewCardScale, InCard.CardImage->GetSizeY() * ViewCardScale);
	NewStyle.Pressed.TintColor = FLinearColor(0.5f, 0.5f, 0.5f, 1.0f);
	CardButton->SetStyle(NewStyle);


	//AddDynamic����Blueprint�ƘA�g���\(Add�o�͕s��)
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
