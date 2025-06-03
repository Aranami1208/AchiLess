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

FCardData UCharacterData::GetDeck(int32 Index)
{
	return Deck[Index];
}
TArray<FCardData> UCharacterData::GetDeckAll()
{
	return Deck;
}
UFUNCTION(BlueprintCallable, Category = "CharacterData")