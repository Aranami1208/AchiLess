// Fill out your copyright notice in the Description page of Project Settings.
#include "Class_AchiLess.h"

//スプリングアーム（カメラ用のコンポーネント）
#include "GameFramework/SpringArmComponent.h"
//カメラのコンポーネント
#include "Camera/CameraComponent.h"

#include "TimerManager.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

#include "GameFramework/PlayerController.h"

#include "ADataManager.h" 
#include "CharacterData.h"

#include "TargetingFunction.h"

#include "BaseCardSkill.h"
#include "CardSkillWidget.h"

#include "Engine/GameInstance.h"
#include "CharacterData.h"



// Sets default values
AClass_AchiLess::AClass_AchiLess() :
	CameraSpringArm(nullptr),
	Camera(nullptr),
	CurrentBoost(0.0f), // 初期化
		CurrentMouseXInput(0.0f),
	CurrentMouseYInput(0.0f),
	bIsAIControll(false),
	InvincibleSec(0.3)
{
	//最大カード数
	int32 DeckSize = 8;
	//デッキの配列の要素数を固定
	DeckData.SetNum(DeckSize);
	CardSkills.SetNum(DeckSize);
	SkillWidgets.SetNum(DeckSize);

	//毎フレームTick()を呼ぶ処理
	PrimaryActorTick.bCanEverTick = true;

	//SpringArmの設定
	//スプリングアームコンポーネントの生成
	CameraSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraSpringArm"));


	CameraSpringArm->SetupAttachment(RootComponent);//ルートコンポーネントにアタッチ？
	CameraSpringArm->TargetArmLength = 2000.f;//対象のオブジェクトからの距離 
	CameraSpringArm->SetRelativeLocation(FVector(0.f, 0.f, 1000.f));//デフォルトのカメラの位置
	CameraSpringArm->SetRelativeRotation(FRotator(-10.f, 180.f, 0.f));//デフォルトのカメラのローテーション
	
	CameraSpringArm->bUsePawnControlRotation = false;
	


	//機体の回転とカメラの動きを独立させる設定
	//CameraSpringArm->bInheritPitch = false;
	//CameraSpringArm->bInheritRoll = false;
	//CameraSpringArm->bInheritYaw = true;
	CameraSpringArm->SetUsingAbsoluteRotation(true);


	//カメラコンポーネントの生成
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));

	Camera->SetupAttachment(CameraSpringArm);//スプリングアームにカメラをアタッチ

	MaxLockOnDistance = 500000.0f;
	LockOnFOV =0.9f;
	LockOnCheckInterval = 0.1;

	
		
}
// Called when the game starts or when spawned
void AClass_AchiLess::BeginPlay()
{
	Super::BeginPlay();

	PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);



	//視野角を設定
	Camera->FieldOfView = 90;


	UCharacterData* CharacterData = Cast<UCharacterData>(UGameplayStatics::GetGameInstance(GetWorld()));

	if (CharacterData)
	{
		//パラメータを取得
		MyParameter = CharacterData->GetParameter();
	}
	else//データが読み込めなかった場合はスピードタイプ
	{
		UADataManager * DataManager = NewObject<UADataManager>();

		DataManager->ReadJsonData("TypeSpeed.json", MyParameter);
	}

	

	//敵だった場合
	if (bIsAIControll)
	{
		UADataManager* DataManager = NewObject<UADataManager>();
		//敵機として設定されているデータを読み込む
		DataManager->ReadJsonData(CharacterData->EnemyAchiLessName + ".json", MyParameter);
		//プレイヤーの場合はチーム0
		Team = Enemy;
	}
	else
	{
		//プレイヤーの場合はチーム0
		Team = Player;
	}

	//最初の速度を設定
	PreviousVelocity = GetActorRotation().Vector() *  MyParameter.MinSpeed;
	CurrentAcceleration = PreviousVelocity;

	//ブーストを初期化
	CurrentBoost = MyParameter.MaxBoost;

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

	// 定期的にロックオンチェックを行うための設定
	GetWorldTimerManager().SetTimer(LockOnCheckTimerHandle, this, &AClass_AchiLess::CheckOnTarget, LockOnCheckInterval, true);

	DeckData = CharacterData->GetDeckAll();

	//デッキの初期化処理
	InitDeck();

	//UE_DEBUG_BREAK();
	
}

void AClass_AchiLess::Beam()
{
	if (!LockOnTargetFigter)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		ABeam* beam = GetWorld()->SpawnActor<ABeam>(BeamClass, GetActorLocation(), FighterMesh->GetComponentRotation(),SpawnParams);

	}
	else//ロックオンしている時は偏差撃ちする
	{
		//UKismetSystemLibrary::PrintString(this, "<<<LockOnTarget>>>");

		FVector MyLocation = GetActorLocation();
		FVector EnemyLocation = LockOnTargetFigter->GetActorLocation();

		FVector EnemyVelocity = LockOnTargetFigter->FighterMesh->GetComponentRotation().Vector()*LockOnTargetFigter->GetCurrentSpeed();

		FVector EnemyAcceleration = LockOnTargetFigter->CurrentAcceleration;

		float BeamProjectileSpeed = 100000;

		//角度を予測する
		FRotator ToTarget = UTargetingFunction::CalcToPreTargetRotation(GetWorld(), this, BeamProjectileSpeed, EnemyLocation, EnemyVelocity,EnemyAcceleration);
		
		//計算した方向に発射
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;

		ABeam* beam = GetWorld()->SpawnActor<ABeam>(BeamClass, GetActorLocation(), ToTarget,SpawnParams);
		
	}
}

// Called every frame
void AClass_AchiLess::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!CameraSpringArm)return;
	//スプリングアームの処理
	FRotator SpringArmRotation = CameraSpringArm->GetComponentRotation();

	SpringArmRotation.Pitch += CurrentMouseYInput * CameraPitchSpeed * DeltaTime;
	SpringArmRotation.Yaw += CurrentMouseXInput * CamerayawSpeed * DeltaTime;
	SpringArmRotation.Roll = 0;

	DTime = DeltaTime;


	CameraSpringArm->SetWorldRotation(SpringArmRotation);

	//AIの操作の時はアームスプリングベースの処理をスキップする
	if(!bIsAIControll)
	UpdateAchiLessRotation(SpringArmRotation);

	FVector Forward = FighterMesh->GetComponentRotation().Vector();//進行方向ベクトルを取得する
	Velocity = Forward * CurrentSpeed * BoostRate * SpeedBuffRate;//スピードを掛けた移動量

	//UKismetSystemLibrary::PrintString(this, "SpeedBuff" + FString::SanitizeFloat(SpeedBuffRate));

	//移動と衝突判定処理 
	AddActorWorldOffset(Velocity * DeltaTime, true);

	
	if (!bIsAcceleration) {

		//加速していないときの処理
		CurrentSpeed = FMath::Clamp(CurrentSpeed - (MyParameter.AirFriction * GetWorld()->GetDeltaSeconds()), MyParameter.MinSpeed, MyParameter.MaxSpeed);
	}

	//ブーストしていないときの処理
	if (!bIsBoosting)
	{
		
		CurrentBoost = FMath::Clamp(CurrentBoost + BoostCost, 0, MyParameter.MaxBoost);
		
		//速度を通常時の状態に戻す
		BoostRate = FMath::Clamp( BoostRate - 0.01f,1.0f,10.f);

		//ブーストゲージがマックス出ないときはスキップ
		if (CurrentBoost != MyParameter.MaxBoost)return;

		//ブーストロックを解除
		if (BoostLock) BoostLock = false;
	}
	else
	{
		InvincibleCount += DeltaTime;
	}

	//無敵時間を超えたら無敵状態フラグを下げる
	if (InvincibleCount >= InvincibleSec) bIsInvencible = false;


	//すべてのカードのクールタイム処理
	for (UBaseCardSkill* Skill : CardSkills)
	{
		if (!Skill)continue;
		Skill->UpdateCooDown(DeltaTime);
	}
	

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
	

	PreviousVelocity = CurrentFrameVelocity; // 次のフレームのために現在の速度を保存
	
	//前回との差分を取りたいため毎フレームリセット
	CurrentMouseXInput = 0.0f;
	CurrentMouseYInput = 0.0f;

}

// Called to bind functionality to input
void AClass_AchiLess::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	//PlayerInputComponent->BindAxis("Pitch", this, &AClass_AchiLess::Pitch);
	//PlayerInputComponent->BindAxis("Yaw", this, &AClass_AchiLess::Yaw);
	//PlayerInputComponent->BindAxis("Roll", this, &AClass_AchiLess::Roll);
	//PlayerInputComponent->BindAxis("Accelerate", this, &AClass_AchiLess::Accelerate);
}

void AClass_AchiLess::InitDeck()
{
	int32 DeckSize = 8;
	int32 Half = 2;
	int32 Index = 0;


	if (DeckData.Num() <= 0)return;
	for (FCardData Card : DeckData)
	{
		if (!Card.CardSkillClass)
		{
			//スキルクラスが見つからなかった場合はセットしない
			UKismetSystemLibrary::PrintString(this, "Skill" + FString::FromInt(Index) + "IsSkillClassNotFound");
			Index++;
			continue;
		}
		UBaseCardSkill* CardSkill = NewObject<UBaseCardSkill>(this, Card.CardSkillClass);
		//Cardのスキルクラスからインスタンスを生成
		CardSkills[Index] = CardSkill;

		//カードデータをセット
		CardSkill->CardData = Card;
		CardSkill->Owner = this;

		//スキルにウィジェットをセット
		CardSkill->Widget = SkillWidgets[Index];


		Index++;
	}
	
	for (Index; Index < DeckSize; Index++)
	{
		if (!EmptyData.CardSkillClass)break;
		UBaseCardSkill* CardSkill = NewObject<UBaseCardSkill>(this, EmptyData.CardSkillClass);
		//Cardのスキルクラスからインスタンスを生成
		CardSkills[Index] = CardSkill;

		//カードデータをセット
		CardSkill->CardData = EmptyData;
		CardSkill->Owner = this;

		//スキルにウィジェットをセット
		CardSkill->Widget = SkillWidgets[Index];
		//UKismetSystemLibrary::PrintString(this, "SetSkillIndex:" + FString::FromInt(Index));
	}

}

void AClass_AchiLess::Pitch(float Value)
{
	//Value = FMath::Clamp(Value,- MyParameter.MaxRotationSpeed, MyParameter.MaxRotationSpeed);

	CurrentMouseYInput = Value;
}

void AClass_AchiLess::Yaw(float Value)
{
	
	CurrentMouseXInput = Value;

}

void AClass_AchiLess::Roll(float Value)
{
	
}


void AClass_AchiLess::SelectSkill1()
{
	SkillIndex = 0;
	SelectCardData =  CardSkills[SkillIndex + UseDeck]->CardData;
}

void AClass_AchiLess::SelectSkill2()
{
	SkillIndex = 1;
	SelectCardData = CardSkills[SkillIndex + UseDeck]->CardData;
}

void AClass_AchiLess::SelectSkill3()
{
	SkillIndex = 2;
	SelectCardData = CardSkills[SkillIndex + UseDeck]->CardData;
}

void AClass_AchiLess::SelectSkill4()
{
	SkillIndex = 3;
	SelectCardData = CardSkills[SkillIndex + UseDeck]->CardData;
}

void AClass_AchiLess::ExecuteSkill()
{
	UE_DEBUG_BREAK();
	UKismetSystemLibrary::PrintString(this, "SkillIndex:" + FString::FromInt(SkillIndex));
	//スキルが設定されていないときはスキップ
	if (SkillIndex == -1)return;
	if (!CardSkills[SkillIndex + UseDeck])return;
	UKismetSystemLibrary::PrintString(this, "SkillIndex:" + FString::FromInt(SkillIndex));
	//スキルを使う

	//クールタイム中だったら処理しない
	UKismetSystemLibrary::PrintString(this, "ExecuteSkill");
	if (CardSkills[SkillIndex + UseDeck]->IsOnCoolDown())return;
	CardSkills[SkillIndex + UseDeck]->StartCoolDown();
	CardSkills[SkillIndex+UseDeck]->ExecuteSkill(this);
}

void AClass_AchiLess::ChangeDeck()
{
	int32 HalfDeckSize = 4;
	UKismetSystemLibrary::PrintString(this,"DeckChange");
	
	for (int32 i = 0; i < HalfDeckSize; i++)
	{
		if(CardSkills[i])
		CardSkills[i]->ChangeWidget( SkillWidgets[i + HalfDeckSize]);
		if(CardSkills[i+HalfDeckSize])
		CardSkills[i + HalfDeckSize]->ChangeWidget( SkillWidgets[i]);
	}
	UseDeck = HalfDeckSize;
}

void AClass_AchiLess::ReChangeDeck()
{
	int32 HalfDeckSize = 4;
	UKismetSystemLibrary::PrintString(this, "DeckChange");

	for (int32 i = 0; i < HalfDeckSize; i++)
	{
		if (CardSkills[i])
			CardSkills[i]->ChangeWidget(SkillWidgets[i]);
		if (CardSkills[i + HalfDeckSize])
			CardSkills[i + HalfDeckSize]->ChangeWidget(SkillWidgets[i+HalfDeckSize]);
	}

	UseDeck = 0;
}

AActor* AClass_AchiLess::SpawnSkillActor(TSubclassOf<AActor> SpawnActorClass)
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = this;
	AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(SpawnActorClass, GetActorTransform(), SpawnParams);

	if (SpawnedActor)
	{
		UKismetSystemLibrary::PrintString(this, "SuccessActorSpawned");
		return SpawnedActor;
	}
	else
	{
		UKismetSystemLibrary::PrintString(this, "ActorSpawnFailed");
	}
	return nullptr;
}


void AClass_AchiLess::Boost(float Seconds)
{
	//ブーストロックがかかっているときは処理しない
	if (BoostLock)return;

	//ブースト状態
	bIsBoosting = true;
	bIsInvencible = true;

	CurrentBoost -= BoostCost;

	//下限、使い切ったらブースト処理をしない
	if (CurrentBoost <= 0)
	{
		CurrentBoost = 0;
		BoostLock = true;
		return;
	}

	//押した秒数の割合
	float SecondRate = 0.f;
	float MaxSec = 0.5;

	SecondRate = FMath::Clamp(Seconds / MaxSec, 0.f, 1.f);
	BoostRate = 1.f + (0.5f * SecondRate);
}

void AClass_AchiLess::BoostReleased()
{
	bIsBoosting = false;
}

void HealHP(float Heal);

UWorld* AClass_AchiLess::GetWorldCntext()
{
	return GetWorld();
}

void AClass_AchiLess::PlaySoundEffect(USoundBase* InSound)
{
	if (!InSound)
	{
		UKismetSystemLibrary::PrintString(this, "SoundNotFound");
		return;
	}
	//SEを再生
	UGameplayStatics::PlaySoundAtLocation(this, InSound, GetActorLocation());
}

void AClass_AchiLess::TakeDamage(float InDamage)
{
	//無敵フラグが立っていたらスキップ
	if (bIsInvencible)
	{
		UCharacterData* GameInstance = Cast<UCharacterData>( UGameplayStatics::GetGameInstance(GetWorld()));

		//ブーストでジャストガードした場合にポイントを追加
		int32 AddPoint = 15;

		GameInstance->EvationPoint += AddPoint;
		return;
	}

	float MinCutRate = 0.0f;
	float MaxCutRate = 1.0f;

	//ダメージカットの倍率は0～100％セント
	DamageCutRate = FMath::Clamp(DamageCutRate, MinCutRate, MaxCutRate);

	//ガードによるダメージカット処理
	float Damage = InDamage - (InDamage * DamageCutRate);

	UKismetSystemLibrary::PrintString(this, "Damage:" + FString::SanitizeFloat(Damage));

	Super::TakeDamage(Damage);
}

FVector2D AClass_AchiLess::GetHUDCircleCenterLocation()
{

	if (!PC)return FVector2D::ZeroVector;

	int32 ViewportSizeX;
	int32 ViewportSizeY;
	PC->GetViewportSize(ViewportSizeX, ViewportSizeY); // APlayerControllerのメソッド
	return FVector2D(ViewportSizeX * 0.5f, ViewportSizeY * 0.5f);
	
}

void AClass_AchiLess::CheckOnTarget()
{
	if (!PC)return;

	FVector2D Center = GetHUDCircleCenterLocation();

	FVector WorldLocation;
	FVector WorldDirection;

	if (!PC->DeprojectScreenPositionToWorld(Center.X, Center.Y, WorldLocation, WorldDirection))return;

	//RayTraceの始点終点
	FVector TraceStart = WorldLocation;
	FVector TraceEnd = WorldLocation + WorldDirection * MaxLockOnDistance;

	//HUDの円のサイズに合わせたスフィアで判定を取る

	//視野角の半分
	float HalfFOV = Camera->FieldOfView / 2.0f ;
	float HalfFOVRadian = FMath::DegreesToRadians(HalfFOV);
	float HalfViewportHeight = Center.Y;
	//判定用スフィアの半径
	float LockOnSphereRadius = MaxLockOnDistance * FMath::Tan(HalfFOVRadian) * (HUDCircleRadiusPixel / HalfViewportHeight);
	
	//ロックオンチェック関数
	LockOnTargetFigter =  UTargetingFunction::CheckOnTarget(GetWorld(), this, WorldLocation, WorldDirection, LockOnSphereRadius, MaxLockOnDistance, LockOnFOV);

}
	

