// Fill out your copyright notice in the Description page of Project Settings.


#include "HopeLight.h"
#include "ADataManager.h"

void AHopeLight::Beam()
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	ABeam* beam = GetWorld()->SpawnActor<ABeam>(BeamClass, GetActorLocation(), FighterMesh->GetComponentRotation(), SpawnParams);
}

void AHopeLight::BeginPlay()
{

	UADataManager* DataManager = NewObject<UADataManager>();

	//HopeLightsのデータを読み込む
	DataManager->ReadJsonData(("HopeLights.json"), MyParameter);
	//プレイヤーの場合はチーム0
	Team = Enemy;

	//指定されたファイル名と特定のフォルダパスを結合
	FString ModelFilePath("/Game/Assets/Models/AhiLess");
	FString FullPath = (ModelFilePath / MyParameter.MeshFileName / MyParameter.MeshFileName + "." + MyParameter.MeshFileName);

	//できたパスからメッシュをロード
	UStaticMesh* Mesh = LoadObject<UStaticMesh>(NULL, *FullPath, NULL, LOAD_None, NULL);



	if (!Mesh)
	{
		//メッシュがセットできなかったら

		UE_DEBUG_BREAK();
		return;
	}

	if (!FighterMesh->SetStaticMesh(Mesh))
	{		//メッシュがセットできなかったら
		UE_DEBUG_BREAK();

	}
}

void AHopeLight::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	FVector Forward = FighterMesh->GetComponentRotation().Vector();//進行方向ベクトルを取得する
	Velocity = Forward * CurrentSpeed ;//スピードを掛けた移動量

	//移動と衝突判定処理 
	AddActorWorldOffset(Velocity * DeltaTime, true);


	//加速度の計算
	FVector CurrentFrameVelocity = Velocity; // 現在のフレームの計算されたVelocity

	if (DeltaTime > KINDA_SMALL_NUMBER) // DeltaTimeが0に近い場合を除外
	{
		CurrentAcceleration = (CurrentFrameVelocity - PreviousVelocity) / DeltaTime; // ここで DeltaTime で割る
	}
	else
	{
		CurrentAcceleration = FVector::ZeroVector; // DeltaTimeが0の場合は加速度なしとする
	}
}
