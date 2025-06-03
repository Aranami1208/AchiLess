// Fill out your copyright notice in the Description page of Project Settings.


#include "CardSkillWidget.h"
#include "Components/Image.h"
void UCardSkillWidget::InitCard(FCardData InCard)
{
	Data = InCard;
	if (!Data.CardImage)return;
	Image->SetBrushFromTexture(Data.CardImage.LoadSynchronous());

	Image->SetColorAndOpacity(FLinearColor(1.0, 1.0f, 1.0f, 1.0f));

	if (!CoolTimeGauge)return;
	CoolTimeGauge->SetVisibility(ESlateVisibility::Hidden);
}

void UCardSkillWidget::StartCoolDown()
{
	if (!Data.CardImage)return;
	Image->SetColorAndOpacity(FLinearColor(0.1, 0.1f, 0.1f, 1.0f));

	if (!CoolTimeGauge)return;
	CoolTimeGauge->SetVisibility(ESlateVisibility::Visible);
}

void UCardSkillWidget::EndCoolDown()
{
	if (!Data.CardImage)return;
	Image->SetColorAndOpacity(FLinearColor(1.0, 1.0f, 1.0f, 1.0f));

	if (!CoolTimeGauge)return;
	CoolTimeGauge->SetVisibility(ESlateVisibility::Hidden);
}
