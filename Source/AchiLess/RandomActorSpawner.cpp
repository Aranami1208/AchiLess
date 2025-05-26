// Fill out your copyright notice in the Description page of Project Settings.


#include "RandomActorSpawner.h"
#include "Engine/World.h"
#include "Engine/TriggerBox.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
// Sets default values
ARandomActorSpawner::ARandomActorSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	//ボックスコンポーネントの生成
	SpawnArea = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnArea"));
	//ルートコンポーネントに設定
	RootComponent = SpawnArea;
	//初期サイズ設定
	SpawnArea->SetBoxExtent(FVector(100.0f, 100.0f, 100.0f));
	//コリジョンを設定しない
	SpawnArea->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//ゲーム中は表示しない
	SpawnArea->bHiddenInGame = true;
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
	if (SpawnCount < 1)
	{
		UKismetSystemLibrary::PrintString(this, "有効な値を設定してください");
		return;
	}

	

	UWorld* World = GetWorld();
	if (!World)return;

	//Origin = 原点、Extent = 範囲
	FVector Origin, Extent;

	//原点取得
	Origin = SpawnArea->GetComponentLocation();
	//ボックスのサイズを取得
	Extent = SpawnArea->GetScaledBoxExtent();

	for (int i = 0; i < SpawnCount; i++)
	{
		//指定した範囲からランダムに一点を取得
		FVector SpawnPoint = UKismetMathLibrary::RandomPointInBoundingBox(Origin,Extent);

		//指定された座標をTransformに変換
		FTransform SpawnTransform(FRotator::ZeroRotator, SpawnPoint);

		//指定された座標にアクターを生成
		TObjectPtr<AActor> SpawnedActor = World->SpawnActor<AActor>(SpawnObject, SpawnTransform);
	}
}


