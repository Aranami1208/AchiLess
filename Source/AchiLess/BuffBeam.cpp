// Fill out your copyright notice in the Description page of Project Settings.


#include "BuffBeam.h"
#include "SpaceFighter.h"
#include "Class_AchiLess.h"
#include "TimerManager.h"
#include "Engine/World.h"

void UBuffBeam::ExecuteSkill_Implementation(ASpaceFighter* InTarget)
{
	

	FireIntervalRate = CardData.SkillValue1;
	EffectTime = CardData.SkillValue2;

    if (!Owner)return;
	Owner->BeamFireTimeRate -= FireIntervalRate;
    Owner->StartBeam();
    //タイマーハンドルのセット
    UWorld* World = Owner->GetWorld();
    World->GetTimerManager().SetTimer(
        RestoreTimer,           // タイマーを識別するハンドル
        this,                    // タイマーイベントを受け取るオブジェクト
        &UBuffBeam::RestoreEffect, // 呼び出す関数
        EffectTime,           // タイマーが発火するまでの時間 (秒)
        false                    // trueにすると繰り返し実行、falseだと一度だけ実行
    );
}

void UBuffBeam::RestoreEffect()
{
	Owner->BeamFireTimeRate += FireIntervalRate;
    Owner->StartBeam();
}
