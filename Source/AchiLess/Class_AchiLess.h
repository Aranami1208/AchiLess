// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "SpaceFighter.h"
#include "DataStruct.h"
#include "Class_AchiLess.generated.h"


UCLASS()
class ACHILESS_API AClass_AchiLess : public ASpaceFighter
{
	GENERATED_BODY()

public:
	//コンストラクタ
	AClass_AchiLess();

	void Beam()override;

protected:
	// ゲーム開始時のみ呼ばれるファイル
	virtual void BeginPlay() override;

	

public:


	// 毎フレーム呼ばれる（更新用）
	virtual void Tick(float DeltaTime) override;

	// 入力系コンポーネント
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	//入力関数ブルプリから呼び出し可能

	UFUNCTION(BlueprintCallable)
	void Pitch(float Value);
	UFUNCTION(BlueprintCallable)
	void Yaw(float Value);
	UFUNCTION(BlueprintCallable)
	void Roll(float Value);


	//ブースト
	UFUNCTION(BlueprintCallable)
	void Boost(float Seconds);
	UFUNCTION(BlueprintCallable)
	void BoostReleased();

	float CameraPitchSpeed = 45.0f;
	float CamerayawSpeed = 90.0f;

private:
	

	UPROPERTY(EditAnywhere)
	FString AchilessName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float CurrentBoost;
	
	

	//ブーストの無敵時間
	UPROPERTY(EditAnywhere)
	float InvincibleSec;

	//加速率
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float BoostRate = 1;

	UPROPERTY(EditAnywhere)
	float BoostCost = 2;

	//ブーストを使い切ったときにはロックを掛ける
	UPROPERTY(EditAnywhere)
	bool BoostLock = false;

	UPROPERTY(EditAnywhere)
	bool bIsBoosting = false;
	
	
	UPROPERTY(VisibleAnywhere)
	class USpringArmComponent* CameraSpringArm;

	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* Camera;
	
	float CurrentMouseYInput;
	float CurrentMouseXInput;

	//AIがコントロールしている
	UPROPERTY(EditAnywhere,BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool bIsAIControll;


	//ロックオン対象
	TObjectPtr<ASpaceFighter> LockOnTargetFigter;

	float MaxLockOnDistance;

	float LockOnFOV;

	//ロックオンチェックの間隔
	float LockOnCheckInterval;

	//定期的に処理するためのタイマーハンドル
	FTimerHandle LockOnCheckTimerHandle;

	//HUDの円の中心の画面座標を求める
	FVector2D GetHUDCircleCenterLocation();

	//ターゲティングしているかチェックする関数
	void CheckOnTarget();

	//PlayerController
	APlayerController* PC;

};
