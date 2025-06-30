// Fill out your copyright notice in the Description page of Project Settings.


#include "DebuffSpeedSkill.h"
#include "Class_AchiLess.h"
#include "DebuffSpeed.h"
#include "Kismet/KismetSystemLibrary.h"

void UDebuffSpeedSkill::ExecuteSkill_Implementation(ASpaceFighter* Target)
{
	ADebuffSpeed* SkillActor;
	SkillActor = Cast<ADebuffSpeed>(Owner->SpawnSkillActor(ADebuffSpeed::StaticClass()));
	
	//UKismetSystemLibrary::PrintString(this, "SkillActorSpawned");
	SkillActor->EffectRate = CardData.SkillValue1;
	//UKismetSystemLibrary::PrintString(this, "SkillValue1:" + FString::SanitizeFloat(SkillActor->EffectRate));
	SkillActor->EffectTime = CardData.SkillValue2;
	SkillActor->AddEffect();
}
