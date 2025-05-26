// Fill out your copyright notice in the Description page of Project Settings.


#include "CardDetail.h"
#include "Kismet/KismetSystemLibrary.h"
#include "SelectLevelActor.h"

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
    if (!SelectLevelActor)
    {
        UKismetSystemLibrary::PrintString(this, "NoSelectLevelActor");
        return;
    }
    NextCardData = CardData;
    SelectLevelActor->SetReload();
    
}

void UCardDetail::Close()
{
    SetVisibility(ESlateVisibility::Hidden);
}

FCardData UCardDetail::GetNextCard()
{
    return NextCardData;
}

void UCardDetail::SetNextCard(FCardData InCard)
{
    NextCardData = InCard;
}
