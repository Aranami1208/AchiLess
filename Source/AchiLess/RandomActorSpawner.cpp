// Fill out your copyright notice in the Description page of Project Settings.


#include "RandomActorSpawner.h"
#include "Engine/World.h"
#include "Engine/TriggerBox.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
// Sets default values
ARandomActorSpawner::ARandomActorSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ARandomActorSpawner::BeginPlay()
{
	Super::BeginPlay();

	//アクタを生成
	SpawnActorInBox();
}

void ARandomActorSpawner::SpawnActorInBox()
{
	if (!SpawnObject)
	{
		UKismetSystemLibrary::PrintString(this, "オブジェクトを設定してください");
	}

}


