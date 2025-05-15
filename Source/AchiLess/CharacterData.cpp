// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterData.h"

void UCharacterData::SetParameter(FDataStruct CharaParam)
{
	Parameter = CharaParam;
}

FDataStruct UCharacterData::GetParameter()
{
	return Parameter;
}
