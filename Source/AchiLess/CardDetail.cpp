// Fill out your copyright notice in the Description page of Project Settings.


#include "CardDetail.h"


void UCardDetail::ShowCardDetail(FCardData& InCard)
{
    if (!DetailImage)return;
    if (!NameText)return;
    if (!FravorText)return;
    
    DetailImage->SetBrushFromSoftTexture(InCard.CardImage);
    NameText->SetText(InCard.Name);
    FravorText->SetText(InCard.FravorText);
    CardData = InCard;
    SetVisibility(ESlateVisibility::Visible);
}

void UCardDetail::Confirm()
{
    SetVisibility(ESlateVisibility::Hidden);
    
}

void UCardDetail::Close()
{
    SetVisibility(ESlateVisibility::Hidden);
}
