// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCardSkill.h"
#include "CardSkillWidget.h"



void UBaseCardSkill::ExecuteSkill_Implementation(ASpaceFighter* Target)
{
	StartCoolDown();
}

void UBaseCardSkill::StartCoolDown()
{
	CurrentCooldownTime = CardData.CoolTime;
	if (!Widget)return;
	Widget->StartCoolDown();
}

float UBaseCardSkill::GetCurrentCooldown()
{
	return CurrentCooldownTime;
}

bool UBaseCardSkill::IsOnCoolDown()
{
	return CurrentCooldownTime > 0.0f;
}

void UBaseCardSkill::UpdateCooDown(float DeltaTime)
{
	//クールタイム出ないときはスキップ
	if (CurrentCooldownTime <= 0.0f)return;
	CurrentCooldownTime -= DeltaTime;
	//クールタイムが終わらなければスキップ
	if (CurrentCooldownTime >0.0f)return;
	//クールタイムが0以下の場合は0.0f
	CurrentCooldownTime = 0.0f;
	if (!Widget)return;
	Widget->EndCoolDown();
}
