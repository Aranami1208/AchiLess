// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCardSkill.h"
#include "CardSkillWidget.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/Image.h"

void UBaseCardSkill::ExecuteSkill_Implementation(ASpaceFighter* Target)
{
	
}

void UBaseCardSkill::StartCoolDown()
{
	if (CardData.CoolTime == 0.0f)return;
	CurrentCooldownTime = CardData.CoolTime;
	if (!Widget)return;
	Widget->StartCoolDown();

}

void UBaseCardSkill::ChangeWidget(UCardSkillWidget* InWIdget)
{
	
	
	Widget = InWIdget;

	Widget->InitCard(CardData);

	if (IsOnCoolDown())
	{
		Widget->StartCoolDown();
	}
	else
	{
		Widget->EndCoolDown();
	}
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
	//UKismetSystemLibrary::PrintString(this, "CoolTime");
	//クールタイム出ないときはスキップ
	if (CurrentCooldownTime <= 0.0f)return;
	CurrentCooldownTime -= DeltaTime;

	if (!Widget)return;//ウィジェット側の処理
	UMaterialInstanceDynamic* DynamicMaterial;
	DynamicMaterial = Widget->CoolTimeGauge->GetDynamicMaterial();
	//クールタイムの進捗を計算
	float Percent = CurrentCooldownTime / CardData.CoolTime;
	DynamicMaterial->SetScalarParameterValue(TEXT("Percent"), Percent);

	//クールタイムが終わらなければスキップ
	if (CurrentCooldownTime >0.0f)return;
	//クールタイムが0以下の場合は0.0f
	CurrentCooldownTime = 0.0f;
	
	Widget->EndCoolDown();

}


