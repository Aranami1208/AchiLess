// Fill out your copyright notice in the Description page of Project Settings.
#include "Class_AchiLess.h"

//スプリングアーム（カメラ用のコンポーネント）
#include "GameFramework/SpringArmComponent.h"

//カメラのコンポーネント
#include "Camera/CameraComponent.h"

#include "Kismet/KismetSystemLibrary.h"

#include "ADataManager.h"

// Sets default values
AClass_AchiLess::AClass_AchiLess() :
	AchilessMesh(nullptr),
	CameraSpringArm(nullptr),
	Camera(nullptr),
	bIsAcceleration(false)
{

	//毎フレームTick()を呼ぶ処理
	PrimaryActorTick.bCanEverTick = true;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));

	AchilessMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AchiLessMesh"));
	RootComponent = DefaultSceneRoot;//ルートコンポーネントに設定


	//SpringArmの設定
	//スプリングアームコンポーネントの生成
	CameraSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraSpringArm"));

	//bUseControllerRotationYaw = true;

	CameraSpringArm->SetupAttachment(RootComponent);//ルートコンポーネントにアタッチ？
	CameraSpringArm->TargetArmLength = 2000.f;//対象のオブジェクトからの距離 
	CameraSpringArm->SetRelativeLocation(FVector(0.f, 0.f, 1000.f));//デフォルトのカメラの位置
	CameraSpringArm->SetRelativeRotation(FRotator(-10.f, 0.f, 0.f));//デフォルトのカメラのローテーション
	//スプリングアームの回転許可設定
	CameraSpringArm->bInheritPitch = true;
	CameraSpringArm->bInheritYaw = true;
	CameraSpringArm->bInheritRoll = true;
	//CameraSpringArm->bUsePawnControlRotation = true;


	//カメラコンポーネントの生成
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));

	Camera->SetupAttachment(CameraSpringArm);//スプリングアームにカメラをアタッチ

	AutoPossessPlayer = EAutoReceiveInput::Player0;  // Player0に自動で操作を渡す


	UADataManager::ReadJsonData("TypeBalance.json", parameter);

	FString ModelFilePath("/Game/Assets/Models/AhiLess");
	FString FullPath = (ModelFilePath /parameter.MeshFileName/ parameter.MeshFileName+"."+parameter.MeshFileName);

	UStaticMesh* Mesh = LoadObject<UStaticMesh>(NULL, *FullPath, NULL, LOAD_None, NULL);
	
	if (!Mesh)
	{
		//メッシュがセットできなかったら
		UE_DEBUG_BREAK();
	}

	if (!AchilessMesh->SetStaticMesh(Mesh))
	{
		//メッシュがセットできなかったら
		UE_DEBUG_BREAK();
	}
	AchilessMesh->SetupAttachment(RootComponent);
	//UE_DEBUG_BREAK();
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

	
	if(bIsAcceleration)return;

	//加速していないときの処理
	CurrentSpeed = FMath::Clamp(CurrentSpeed - (parameter.AirFriction * GetWorld()->GetDeltaSeconds()), parameter.MinSpeed, parameter.MaxSpeed);


}

// Called to bind functionality to input
void AClass_AchiLess::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	//PlayerInputComponent->BindAxis("Pitch", this, &AClass_AchiLess::Pitch);
	//PlayerInputComponent->BindAxis("Yaw", this, &AClass_AchiLess::Yaw);
	//PlayerInputComponent->BindAxis("Roll", this, &AClass_AchiLess::Roll);
	//PlayerInputComponent->BindAxis("Accelerate", this, &AClass_AchiLess::Accelerate);
	

}

void AClass_AchiLess::Pitch(float Value)
{
	Value = FMath::Clamp(Value,- parameter.MaxRotationSpeed, parameter.MaxRotationSpeed);
	//ピッチ操作
	AddActorLocalRotation(FRotator(Value * parameter.TurnSpeed * GetWorld()->GetDeltaSeconds(), 0.f, 0.f));
}

void AClass_AchiLess::Yaw(float Value)
{
	//AddActorLocalRotation(FRotator(0.f, Value * TurnSpeed * GetWorld()->GetDeltaSeconds(), 0.f));
}

void AClass_AchiLess::Roll(float Value)
{
	AddActorLocalRotation(FRotator(0.f, 0.f, Value * parameter.TurnSpeed * GetWorld()->GetDeltaSeconds()));
}

void AClass_AchiLess::Accelerate(float Value)
{
	//Clampは範囲制限
	bIsAcceleration = true;
	CurrentSpeed = FMath::Clamp(CurrentSpeed + (Value * parameter.Accelerate * GetWorld()->GetDeltaSeconds()), parameter.MinSpeed, parameter.MaxSpeed);
	
}

void AClass_AchiLess::AcceleReleased()
{
	bIsAcceleration = false;
}


