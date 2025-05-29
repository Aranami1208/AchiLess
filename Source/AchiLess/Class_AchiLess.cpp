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


// Sets default values
AClass_AchiLess::AClass_AchiLess() :
	CameraSpringArm(nullptr),
	Camera(nullptr),
	CurrentBoost(0.0f), // 初期化
		CurrentMouseXInput(0.0f),
	CurrentMouseYInput(0.0f),
	bIsAIControll(false)
{


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

	//UE_DEBUG_BREAK();
	
}

void AClass_AchiLess::Beam()
{
	if (!LockOnTargetFigter)
	{
		ABeam* beam = GetWorld()->SpawnActor<ABeam>(BeamClass, GetActorLocation(), FighterMesh->GetComponentRotation());
	}
	else//ロックオンしている時は偏差撃ちする
	{
		UKismetSystemLibrary::PrintString(this, "<<<LockOnTarget>>>");

		FVector MyLocation = GetActorLocation();
		FVector EnemyLocation = LockOnTargetFigter->GetActorLocation();

		FVector EnemyVelocity = LockOnTargetFigter->GetVelocity();

		float BeamProjectileSpeed = 100000;

		FVector PredictedEnemyLocation = EnemyLocation;
		float TravelTime = 0.0f;
		const int32 MaxIterations = 10; // 予測の反復回数
		const float ToleranceSq = FMath::Square(50.0f); // 許容誤差 (単位: cm)

		for (int32 i = 0; i < MaxIterations; ++i)
		{
			// 前回予測した到達時間で、敵がどこにいるかを予測
			FVector NextPredictedEnemyLocation = EnemyLocation + (EnemyVelocity * TravelTime);
			float DistanceToPredictedTarget = FVector::DistSquared(MyLocation, NextPredictedEnemyLocation); // 距離の2乗で計算（平方根計算を避けるため）

			// 予測位置までの距離から、弾が到達するのにかかる新しい時間を計算
			float NextTravelTime = FMath::Sqrt(DistanceToPredictedTarget) / BeamProjectileSpeed;

			// 予測時間の変化が許容範囲内であれば、収束したとみなす
			if (FMath::Abs(NextTravelTime - TravelTime) < ToleranceSq) // ここも距離の許容誤差と合わせる
			{
				PredictedEnemyLocation = NextPredictedEnemyLocation;
				break;
			}

			TravelTime = NextTravelTime;
			PredictedEnemyLocation = NextPredictedEnemyLocation; // 予測位置を更新

			// デバッグ表示 (予測位置)
			// DrawDebugSphere(GetWorld(), PredictedEnemyLocation, 100.0f, 12, FColor::Yellow, false, 0.1f);
		}

		FVector BeamVec = (PredictedEnemyLocation - MyLocation).GetSafeNormal();

		//計算した方向に発射
		ABeam* beam = GetWorld()->SpawnActor<ABeam>(BeamClass, GetActorLocation(), BeamVec.Rotation());
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
	Velocity = Forward * CurrentSpeed * BoostRate;//スピードを掛けた移動量

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

	//全開との差分を取りたいため毎フレームリセット
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


void AClass_AchiLess::Boost(float Seconds)
{
	//ブーストロックがかかっているときは処理しない
	if (BoostLock)return;

	//ブースト状態
	bIsBoosting = true;

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
	

	TArray<FHitResult> HitResults;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	QueryParams.bTraceComplex = false;
	
	
	bool bHit = UKismetSystemLibrary :: SphereTraceMulti(
		GetWorld(),                 // WorldContextObject
		TraceStart,                 // Start
		TraceEnd,                   // End
		LockOnSphereRadius,  // Radius
		ETraceTypeQuery::TraceTypeQuery1, // TraceChannel: ここを適切なコリジョンチャネルに設定する
		// 例: ETraceTypeQuery::TraceTypeQuery2 (Visibility) など
		// プロジェクトのコリジョン設定に合わせて変更してください。
		false,                      // bTraceComplex
		TArray<AActor*>(),          // ActorsToIgnore: QueryParamsで設定済みなので空でもOK
		EDrawDebugTrace::None, // DrawDebugType: デバッグ表示の種類
		HitResults,                 // OutHits
		true,                       // bIgnoreSelf: QueryParamsで設定済み
		FLinearColor::Red,          // TraceColor
		FLinearColor::Green,        // TraceHitColor
		LockOnCheckInterval - 0.01f // DrawTime
	);
	DrawDebugSphere(GetWorld(), TraceEnd, LockOnSphereRadius, 16, FColor::Blue, false, LockOnCheckInterval - 0.01f);

	LockOnTargetFigter = nullptr;

	//ヒットした場合の処理
	if (!bHit)return;
	
	//中心に一番近い値を持っておく
	float MinDotProduct = -1.0f;
	//接触したオブジェクトをすべて判定
	for (const FHitResult HitResult : HitResults)
	{
		ASpaceFighter* HitFighter = Cast<ASpaceFighter>(HitResult.GetActor());

		//キャストに失敗したらスキップ
		if (!HitFighter)continue;
		
		//UKismetSystemLibrary::PrintString(this, "HitFighter");
		// ここから視野角チェック

		//カメラの前方
		FVector PlayerForwardVector = WorldDirection;
		//敵機への方向ベクトル
		FVector DirectionToEnemy = (HitFighter->GetActorLocation() - GetActorLocation()).GetSafeNormal(); 

		float DotProduct = FVector::DotProduct(PlayerForwardVector, DirectionToEnemy);

		//ロックオン範囲内でなければスキップ
		if (DotProduct < LockOnFOV)continue;
		//最も中心に近いものでなければスキップ
		if (DotProduct <= MinDotProduct)continue;
		
		UKismetSystemLibrary::PrintString(this, "LockSuccess");
		MinDotProduct = DotProduct;
		LockOnTargetFigter = HitFighter;

		//設定したターゲットと自機の間にオブジェクトがある場合、ターゲットを外す

		FVector RayTraceStart = GetActorLocation();
		FVector RayTraceEnd = HitFighter->GetActorLocation();

		FHitResult RayResult;

		FCollisionQueryParams RayTraceQueryParams;
		RayTraceQueryParams.AddIgnoredActor(this);
		RayTraceQueryParams.AddIgnoredActor(HitFighter);
		//精度を高く
		RayTraceQueryParams.bTraceComplex = true;

		bool bHitRay = GetWorld()->LineTraceSingleByChannel(
			RayResult,
			RayTraceStart,
			RayTraceEnd,
			ECollisionChannel::ECC_Visibility,
			RayTraceQueryParams
		);

		//障害物がなければスキップ
		if (!bHitRay)continue;
		
		ABeam* HitBeam = Cast<ABeam>(RayResult.GetActor());
		
		//障害物がビームであった場合スキップ
		if (HitBeam)continue;

		//障害物があり、ビームではない場合さえぎられているのでターゲットを外す
		LockOnTargetFigter = nullptr;
			
	}
}
	

