// Fill out your copyright notice in the Description page of Project Settings.


#include "DebuffSpeed.h"
#include "TargetingFunction.h"
#include "Class_AchiLess.h" 
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
ADebuffSpeed::ADebuffSpeed()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	
}

// Called when the game starts or when spawned
void ADebuffSpeed::BeginPlay()
{
	Super::BeginPlay();
	
}

void ADebuffSpeed::AddEffect()
{
	User = Cast<AClass_AchiLess>(Owner);
	if (!User)
	{
		UKismetSystemLibrary::PrintString(this, "NoOwner");
		return;
	}
	//UKismetSystemLibrary::PrintString(this, "UseDebuffSpeed");
	TargetActor = Cast<AClass_AchiLess>(User->LockOnTargetFigter);

	if (!TargetActor)return;
	TargetActor->SpeedBuffRate = TargetActor->SpeedBuffRate - EffectRate;
	//UKismetSystemLibrary::PrintString(this, "EffectRate:" + FString::SanitizeFloat(EffectRate));
	//UKismetSystemLibrary::PrintString(this, "SpeedBuffRate:" + FString::SanitizeFloat(TargetActor->SpeedBuffRate));
	//タイマーハンドルのセット
	GetWorld()->GetTimerManager().SetTimer(
		RestoreTimer,           // タイマーを識別するハンドル
		this,                    // タイマーイベントを受け取るオブジェクト
		&ADebuffSpeed::RestoreEffect, // 呼び出す関数
		EffectTime,           // タイマーが発火するまでの時間 (秒)
		false                    // trueにすると繰り返し実行、falseだと一度だけ実行
	);

}

void ADebuffSpeed::RestoreEffect()
{
	UKismetSystemLibrary::PrintString(this, "RestoreDebuff");
	//if (!TargetActor)return;
	TargetActor->SpeedBuffRate = TargetActor->SpeedBuffRate + EffectRate;
	Destroy();
}

// Called every frame
void ADebuffSpeed::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

