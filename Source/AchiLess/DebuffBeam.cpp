// Fill out your copyright notice in the Description page of Project Settings.


#include "DebuffBeam.h"
#include "Kismet/KismetSystemLibrary.h"


void UDebuffBeam::ExecuteSkill_Implementation(ASpaceFighter* Target)
{
	AddInterval = CardData.SkillValue1;
	EffectTime = CardData.SkillValue2;
	//UKismetSystemLibrary::PrintString(this, "UseDebuff");
	//ビームを撃つ感覚を長くする
	TargetActor = Cast<AClass_AchiLess>( Owner->LockOnTargetFigter);

	//ターゲットがAchiLess出なかったらスキップ
	if (!TargetActor)return;

	TargetActor->BeamFireTimeRate += AddInterval;
	TargetActor->StartBeam();
	
	//タイマーハンドルのセット
	GetWorld()->GetTimerManager().SetTimer(
		TimerHandle,           // タイマーを識別するハンドル
		this,                    // タイマーイベントを受け取るオブジェクト
		&UDebuffBeam::RestoreEffect, // 呼び出す関数
		EffectTime,           // タイマーが発火するまでの時間 (秒)
		false                    // trueにすると繰り返し実行、falseだと一度だけ実行
	);
}

void UDebuffBeam::RestoreEffect()
{
	if (!TargetActor)return;
	
	TargetActor->BeamFireTimeRate -= AddInterval;
	TargetActor->StartBeam();
	UKismetSystemLibrary::PrintString(this, "RestoreEffect");
}
