// Fill out your copyright notice in the Description page of Project Settings.


#include "SpaceFighter.h"
#include "ADataManager.h"

#include "TimerManager.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"


#include "ADataManager.h" 
#include "CharacterData.h"

#include "DrawDebugHelpers.h"


// Sets default values
ASpaceFighter::ASpaceFighter():
FighterMesh(nullptr),
bIsAcceleration(false),
CurrentSpeed(0.0f), // 初期化
CurrentHp(0.0f)// 初期化
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	
	FighterMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FighterMesh"));
	RootComponent = DefaultSceneRoot;//ルートコンポーネントに設定

	AutoPossessPlayer = EAutoReceiveInput::Player0;  // Player0に自動で操作を渡す

	FighterMesh->SetupAttachment(DefaultSceneRoot);


}

// Called when the game starts or when spawned
void ASpaceFighter::BeginPlay()
{
	Super::BeginPlay();
	
	UADataManager* DataManager = NewObject<UADataManager>();

	DataManager->ReadJsonData("TypeSpeed.json", MyParameter);

	//HPを初期化
	CurrentHp = MyParameter.MaxHp;

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
	{
		UKismetSystemLibrary::PrintString(this, "Could not set mesh");
		//メッシュがセットできなかったら
		UE_DEBUG_BREAK();

	}
}

// Called every frame
void ASpaceFighter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector Forward = FighterMesh->GetComponentRotation().Vector();//進行方向ベクトルを取得する
	Velocity = Forward * CurrentSpeed ;//スピードを掛けた移動量

	//移動と衝突判定処理 
	AddActorWorldOffset(Velocity * DeltaTime, true);


	if (!bIsAcceleration) {

		//加速していないときの処理
		CurrentSpeed = FMath::Clamp(CurrentSpeed - (MyParameter.AirFriction * GetWorld()->GetDeltaSeconds()), MyParameter.MinSpeed, MyParameter.MaxSpeed);
	}


}

// Called to bind functionality to input
void ASpaceFighter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ASpaceFighter::UpdateAchiLessRotation(const FRotator TargetRotation)
{

	if (!FighterMesh)return;

	//現在のローテーション
	FRotator CurrentAchiLessRotation = FighterMesh->GetComponentRotation();
	//Yawは維持するためにいったんコピー
	FRotator TargetAchiLessRotation = CurrentAchiLessRotation;

	TargetAchiLessRotation.Pitch = TargetRotation.Pitch;
	TargetAchiLessRotation.Yaw = TargetRotation.Yaw;


	//ターゲット方向のベクトルを取得
	FVector TargetForwardHorizontal = TargetRotation.Vector();


	//正規化
	if (TargetForwardHorizontal.IsNearlyZero())return;
	TargetForwardHorizontal.Normalize();

	//デバッグ処理（ターゲット方向を描画）
	
	
	FVector Start = FighterMesh->GetComponentLocation();
	FVector End = Start + TargetForwardHorizontal * 50000;
	

	// 赤色の矢印を描画
	UKismetSystemLibrary::DrawDebugArrow(
		GetWorld(),          // ワールド
		Start,       // 開始点
		End,         // 終了点
		100.0f,              // 矢じりのサイズ
		FColor::Red,         // 色
		-1.0f,               // 描画時間 (0か-1なら1フレーム)
		10.0f                // 線の太さ
	);

	

	//自機の方向ベクトルを取得
	FVector AchiLessForwardHorizontal = CurrentAchiLessRotation.Vector();

	//正規化
	if (AchiLessForwardHorizontal.IsNearlyZero())return;
	AchiLessForwardHorizontal.Normalize();

	//内積を取る
	float Dot = FVector::DotProduct(AchiLessForwardHorizontal, TargetForwardHorizontal);
	Dot = FMath::Clamp(Dot, -1.0f, 1.0f);

	//外積を取る
	FVector Cross = FVector::CrossProduct(AchiLessForwardHorizontal, TargetForwardHorizontal);

	
	float AngleDifferenceDegrees = FMath::RadiansToDegrees(acosf(Dot));

	if (Cross.Z > 0.0f)
	{
		AngleDifferenceDegrees *= -1.0;
	}

	//回転速度制限系
	float MaxRollFromYaw = MyParameter.MaxRotationSpeed > 0 ? MyParameter.MaxRotationSpeed * 20.0f : 45.0f;
	float TargetRollForAiming = FMath::Clamp(AngleDifferenceDegrees * -1.0f, -MaxRollFromYaw, MaxRollFromYaw);

	TargetAchiLessRotation.Roll = TargetRollForAiming;

	FRotator NewRotation = FMath::RInterpTo(CurrentAchiLessRotation, TargetAchiLessRotation, DTime, MyParameter.MaxRotationSpeed);
	FighterMesh->SetWorldRotation(NewRotation);

}

void ASpaceFighter::Accelerate(float Value)
{
	//Clampは範囲制限
	bIsAcceleration = true;
	CurrentSpeed = FMath::Clamp(CurrentSpeed + (Value * MyParameter.Accelerate * GetWorld()->GetDeltaSeconds()), MyParameter.MinSpeed, MyParameter.MaxSpeed);

}

void ASpaceFighter::AcceleReleased()
{
	bIsAcceleration = false;
}

void ASpaceFighter::Beam()
{
	ABeam* beam = GetWorld()->SpawnActor<ABeam>(BeamClass, GetActorLocation(), FighterMesh->GetComponentRotation());
	UKismetSystemLibrary::PrintString(this, "BeamOwner = "+this->GetName());
	beam->Owner = this;
}

void ASpaceFighter::TakeDamage(float InDamage)
{
	//HP減少処理
	CurrentHp = FMath::Clamp(CurrentHp -= InDamage, 0, MyParameter.MaxHp);
}

void ASpaceFighter::StartBeam()
{
	//押した瞬間に一発撃つ
	Beam();

	float BeamInterval = 0.1f * BeamFireTimeRate;
	UKismetSystemLibrary::PrintString(this, "Rate:" + FString::SanitizeFloat(BeamFireTimeRate));
	GetWorldTimerManager().SetTimer(BeamTimerHandle, this, &ASpaceFighter::Beam, BeamInterval, true);
}

void ASpaceFighter::StopBeam()
{
	GetWorldTimerManager().ClearTimer(BeamTimerHandle);
}

FVector ASpaceFighter::GetCurrentVelocity()
{
	return Velocity;
}

void ASpaceFighter::HealHP(float Heal)
{
	CurrentHp = FMath::Clamp(CurrentHp + Heal, 0, MyParameter.MaxHp);
}

