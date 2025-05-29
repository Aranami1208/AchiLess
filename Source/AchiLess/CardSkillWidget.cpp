// Fill out your copyright notice in the Description page of Project Settings.


#include "CardSkillWidget.h"
#include "Components/Image.h"
void UCardSkillWidget::InitCard(FCardData InCard)
{
	Data = InCard;
	Image->SetBrushFromTexture(Data.CardImage.LoadSynchronous());
}
