// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterData.h"
#include "CardItemWidget.h"

void UCharacterData::SetParameter(FDataStruct CharaParam)
{
	Parameter = CharaParam;
}

FDataStruct UCharacterData::GetParameter()
{
	return Parameter;
}

void UCharacterData::SetDeckFromData(TArray<UCardItemWidget*> InData)
{
	for (UCardItemWidget* Card : InData)
	{
		Deck.Add(Card->GetCardData());
	}
}
