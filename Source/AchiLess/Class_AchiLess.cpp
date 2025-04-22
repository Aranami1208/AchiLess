// Fill out your copyright notice in the Description page of Project Settings.


#include "Class_AchiLess.h"

// Sets default values
AClass_AchiLess::AClass_AchiLess()
{
 	//毎フレームTick()を呼ぶ処理
	PrimaryActorTick.bCanEverTick = true;
	

	AchilessMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AchiLessMesh"));
	RootComponent = AchilessMesh;//ルートコンポーネントに設定

	AutoPossessPlayer = EAutoReceiveInput::Player0;  // Player0に自動で操作を渡す

	//初期速度
	MaxSpeed = 3000.f;
	CurrentSpeed = 0.f;

}

// Called when the game starts or when spawned
void AClass_AchiLess::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AClass_AchiLess::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector Forward = GetActorForwardVector();//進行方向ベクトルを取得する
	Velocity = Forward * CurrentSpeed;//スピードを掛けた移動量

	//移動と衝突判定処理 
	AddActorWorldOffset(Velocity * DeltaTime, true);

}

// Called to bind functionality to input
void AClass_AchiLess::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	PlayerInputComponent->BindAxis("Pitch", this, &AClass_AchiLess::Pitch);
	PlayerInputComponent->BindAxis("Yaw", this, &AClass_AchiLess::Yaw);
	PlayerInputComponent->BindAxis("Accelerate", this, &AClass_AchiLess::Accelerate);

}

void AClass_AchiLess::Pitch(float Value)
{
	//ピッチ操作
	AddActorLocalRotation(FRotator(Value * TurnSpeed * GetWorld()->GetDeltaSeconds(), 0.f, 0.f));
}

void AClass_AchiLess::Yaw(float Value)
{
	AddActorLocalRotation(FRotator(0.f, Value * TurnSpeed * GetWorld()->GetDeltaSeconds(), 0.f));
}

void AClass_AchiLess::Accelerate(float Value)
{
	//Clampは範囲制限
	CurrentSpeed = FMath::Clamp(CurrentSpeed + (Value * Acceleration * GetWorld()->GetDeltaSeconds()), 0.f, MaxSpeed);
}

