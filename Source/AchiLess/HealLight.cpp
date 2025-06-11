// Fill out your copyright notice in the Description page of Project Settings.


#include "HealLight.h"
#include "Class_AchiLess.h"
#include "Kismet/KismetSystemLibrary.h"
void UHealLight::ExecuteSkill_Implementation(class ASpaceFighter* Target)
{
	//HP‚ð{SkillValue1}“‰ñ•œ
	float Heal = Owner->MyParameter.MaxHp * (CardData.SkillValue1 / 100);
	//UKismetSystemLibrary::PrintString(this, FString::SanitizeFloat(Heal));
	Owner->HealHP(Heal);
}
