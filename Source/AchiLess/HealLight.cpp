// Fill out your copyright notice in the Description page of Project Settings.


#include "HealLight.h"
#include "Class_AchiLess.h"

void UHealLight::ExecuteSkill_Implementation(class ASpaceFighter* Target)
{
	//HP‚ð30“‰ñ•œ
	Owner->HealHP(Owner->MyParameter.MaxHp * 0.30f);
}
