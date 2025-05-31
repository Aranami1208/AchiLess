// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCardSkill.h"

void UBaseCardSkill::ExecuteSkill(ASpaceFighter* Target)
{
}

void UBaseCardSkill::StartCoolDown()
{
}

float UBaseCardSkill::GetCurrentCooldown()
{
	return 0.0f;
}

bool UBaseCardSkill::IsOnCoolDown()
{
	return false;
}

void UBaseCardSkill::UpdateCooDown(float DeltaTime)
{
}
