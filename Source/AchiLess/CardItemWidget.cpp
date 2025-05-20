// Fill out your copyright notice in the Description page of Project Settings.


#include "CardItemWidget.h"
#include "Components/Button.h"

void UCardItemWidget::InitCard(const FCardData& InCard)
{
	CardData = InCard;

	FSlateBrush NewBrush;
	NewBrush.SetResourceObject(InCard.CardImage.LoadSynchronous());
	NewBrush.ImageSize = FVector2D(150, 250);
	
	FButtonStyle NewStyle = CardButton->WidgetStyle;

	NewStyle.Normal.SetResourceObject(InCard.CardImage.LoadSynchronous());
	NewStyle.Normal.ImageSize = FVector2D(150.250);

	CardButton->SetStyle(NewStyle);
	
}
