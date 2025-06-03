// Fill out your copyright notice in the Description page of Project Settings.


#include "CardDetail.h"
#include "Kismet/KismetSystemLibrary.h"
#include "SelectLevelActor.h"
#include "Components/Button.h"

void UCardDetail::ShowCardDetail(FCardData& InCard)
{
    if (!DetailImage)return;
    if (!NameText)return;
    if (!FravorText)return;
    if (!ConfirmButton)return;
    if (!ConfirmText)return;
    
    DetailImage->SetBrushFromSoftTexture(InCard.CardImage);
    NameText->SetText(InCard.Name);
    FravorText->SetText(InCard.FravorText);
    CardData = InCard;
    SetVisibility(ESlateVisibility::Visible);

    FButtonStyle NewStyle = ConfirmButton->WidgetStyle;

    if (CardData.IsValid)
    {
        NewStyle.Normal.TintColor = FLinearColor(0.5f, 0.5f, 0.5f, 1.0f);

        NewStyle.Hovered = NewStyle.Normal;
        NewStyle.Pressed = NewStyle.Normal;

        ConfirmButton->SetStyle(NewStyle);

        ConfirmText->SetColorAndOpacity(FLinearColor(0.0f, 0.4, 0.9f, 1.0f));
    }
    else
    {
        NewStyle.Normal.TintColor = FLinearColor(0.2f, 0.2f, 0.2f, 1.0f);

        NewStyle.Hovered = NewStyle.Normal;
        NewStyle.Pressed = NewStyle.Normal;

        ConfirmButton->SetStyle(NewStyle);
       
        ConfirmText->SetColorAndOpacity(FLinearColor(0.1f, 0.1f, 0.1f, 1.0f));
    }

    ConfirmButton->SetStyle(NewStyle);
   
}

void UCardDetail::Confirm()
{
    if (!CardData.IsValid)return;
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
