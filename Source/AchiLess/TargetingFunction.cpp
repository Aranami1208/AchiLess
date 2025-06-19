// Fill out your copyright notice in the Description page of Project Settings.


#include "TargetingFunction.h"
#include "Kismet/KismetSystemLibrary.h"
#include "SpaceFighter.h"

ASpaceFighter* UTargetingFunction::CheckOnTarget(UWorld* WorldContext,ASpaceFighter* Owner,FVector Location, FVector Direction, float Radius, float MaxDistance, float FOV)
{
	ASpaceFighter* LockOnTargetFigter = nullptr;

	AActor* OwnerActor = Cast<AActor>(Owner);

	if (!OwnerActor)return LockOnTargetFigter;

	FVector TraceStart = Location;
	FVector TraceEnd = TraceStart + Direction * MaxDistance;

	TArray<FHitResult> HitResults;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(OwnerActor);
	QueryParams.bTraceComplex = false;


	bool bHit = UKismetSystemLibrary::SphereTraceMulti(
		WorldContext,                 // WorldContextObject
		TraceStart,                 // Start
		TraceEnd,                   // End
		Radius,  // Radius
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
		0.1 - 0.01f // DrawTime
	);

	//デバッグ用スフィア表示
	//DrawDebugSphere(WorldContext, TraceEnd, Radius, 16, FColor::Blue, false, 0.1f - 0.01f);

	LockOnTargetFigter = nullptr;

	//ヒットしていない場合スキップ
	if (!bHit)return LockOnTargetFigter;

	//中心に一番近い値を持っておく
	float MinDotProduct = -1.0f;
	//接触したオブジェクトをすべて判定
	for (const FHitResult HitResult : HitResults)
	{
		ASpaceFighter* HitFighter = Cast<ASpaceFighter>(HitResult.GetActor());

		//キャストに失敗したらスキップ
		if (!HitFighter)continue;

		//UKismetSystemLibrary::PrintString(OwnerActor, "HitFighter");
		// ここから視野角チェック

		
		FVector PlayerForwardVector = Direction;
		//敵機への方向ベクトル
		FVector DirectionToEnemy = (HitFighter->GetActorLocation() - Owner->GetActorLocation()).GetSafeNormal();

		float DotProduct = FVector::DotProduct(PlayerForwardVector, DirectionToEnemy);

		//ロックオン範囲内でなければスキップ
		if (DotProduct < FOV)continue;
		//最も中心に近いものでなければスキップ
		if (DotProduct <= MinDotProduct)continue;

		//UKismetSystemLibrary::PrintString(OwnerActor, "LockSuccess");
		MinDotProduct = DotProduct;
		LockOnTargetFigter = HitFighter;

		//設定したターゲットと自機の間にオブジェクトがある場合、ターゲットを外す

		FVector RayTraceStart = Owner->GetActorLocation();
		FVector RayTraceEnd = HitFighter->GetActorLocation();

		FHitResult RayResult;

		FCollisionQueryParams RayTraceQueryParams;
		RayTraceQueryParams.AddIgnoredActor(OwnerActor);
		RayTraceQueryParams.AddIgnoredActor(HitFighter);
		//精度を高く
		RayTraceQueryParams.bTraceComplex = true;

		bool bHitRay = WorldContext->LineTraceSingleByChannel(
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

	return LockOnTargetFigter;
}

FRotator UTargetingFunction::CalcToPreTargetRotation(UWorld* WorldContext, ASpaceFighter* Owner, float TargetSpeed, FVector TargetLocation, FVector TargetVelocity, FVector TargetAcceleration)
{
	FVector MyLocation = Owner->GetActorLocation();


	FVector PredictedTargetLocation = TargetLocation;
	float TravelTime = 0.0f;
	const int32 MaxIterations = 10; // 予測の反復回数
	const float ToleranceSq = FMath::Square(50.0f); // 許容誤差 (単位: cm)

	

	for (int32 i = 0; i < MaxIterations; ++i)
	{
		// 前回予測した到達時間で、敵がどこにいるかを予測
		//FVector NextPredictedTargetLocation = TargetLocation + (TargetVelocity * TravelTime);
		
		//25.06.19 T.Aranami 追加：加速度を考慮した計算をする
		FVector NextPredictedTargetLocation = TargetLocation + (TargetVelocity * TravelTime) + (0.5f * TargetAcceleration * FMath::Square(TravelTime));

		float DistanceToPredictedTarget = FVector::DistSquared(MyLocation, NextPredictedTargetLocation); // 距離の2乗で計算（平方根計算を避けるため）

		// 予測位置までの距離から、弾が到達するのにかかる新しい時間を計算
		float NextTravelTime = FMath::Sqrt(DistanceToPredictedTarget) / TargetSpeed;

		// 予測時間の変化が許容範囲内であれば、収束したとみなす
		if (FMath::Abs(NextTravelTime - TravelTime) < ToleranceSq) // ここも距離の許容誤差と合わせる
		{
			PredictedTargetLocation = NextPredictedTargetLocation;
			break;
		}

		TravelTime = NextTravelTime;
		PredictedTargetLocation = NextPredictedTargetLocation; // 予測位置を更新

		
	}

	// デバッグ表示 (予測位置)
	//DrawDebugSphere(WorldContext, PredictedTargetLocation, 2000.0f, 12, FColor::Yellow, false, 10.0f);

	FVector BeamVec = (PredictedTargetLocation - MyLocation).GetSafeNormal();

	return BeamVec.Rotation();

}
