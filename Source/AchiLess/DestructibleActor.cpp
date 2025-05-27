// Fill out your copyright notice in the Description page of Project Settings.


#include "DestructibleActor.h"

// Sets default values
ADestructibleActor::ADestructibleActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ADestructibleActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void ADestructibleActor::TakeDamage(float Damage)
{
	//0を下回らないようにする
	CurrentHp = FMath::Max(CurrentHp -= Damage,0);

	//アクターを非表示
	SetActorHiddenInGame(true);
}

int32 ADestructibleActor::GetCurrentHP()
{
	return CurrentHp;
}

// Called every frame
void ADestructibleActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

