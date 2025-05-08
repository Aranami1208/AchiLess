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
		return;
	}
	if (!SpawnArea)
	{
		UKismetSystemLibrary::PrintString(this, "範囲を指定してください");
		return;
	}
	if (SpawnCount < 1)
	{
		UKismetSystemLibrary::PrintString(this, "有効な値を設定してください");
		return;
	}

	UWorld* World = GetWorld();
	if (!World)return;

	//Origin = 原点、Extent = 範囲
	FVector Orign, Extent;

	//設定されたTriggerBoxのサイズを取得
	SpawnArea->GetActorBounds(false, Orign, Extent);

	for (int i = 0; i < SpawnCount; i++)
	{
		//指定した範囲からランダムに一点を取得
		FVector SpawnPoint = UKismetMathLibrary::RandomPointInBoundingBox(Orign,Extent);

		//指定された座標をTransformに変換
		FTransform SpawnTransform(FRotator::ZeroRotator, SpawnPoint);

		//指定された座標にアクターを生成
		TObjectPtr<AActor> SpawnedActor = World->SpawnActor<AActor>(SpawnObject, SpawnTransform);
	}
}


