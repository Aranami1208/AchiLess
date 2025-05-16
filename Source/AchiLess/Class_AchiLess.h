// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "DataStruct.h"
#include "Class_AchiLess.generated.h"


UCLASS()
class ACHILESS_API AClass_AchiLess : public APawn
{
	GENERATED_BODY()

public:
	//コンストラクタ
	AClass_AchiLess();

protected:
	// ゲーム開始時のみ呼ばれるファイル
	virtual void BeginPlay() override;

public:

	//ルートのためのコンポーネント
	UPROPERTY(EditAnywhere)
	TObjectPtr<USceneComponent> DefaultSceneRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> AchilessMesh;

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


	//加速
	UFUNCTION(BlueprintCallable)
	void Accelerate(float Value);

	//自動減速
	UFUNCTION(BlueprintCallable)
	void AcceleReleased();

	//ブースト
	UFUNCTION(BlueprintCallable)
	void Boost(float Seconds);
	UFUNCTION(BlueprintCallable)
	void BoostReleased();
	
private:
	
	FVector Velocity;//移動方向のベクトル

	float MaxRotationSpeed;

	UPROPERTY(EditAnywhere)
	FString AchilessName;

	// privateでもブルプリから読み取りのみ可
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FDataStruct MyParameter;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float CurrentSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float CurrentBoost;
	
	UPROPERTY(EditAnywhere)
	bool bIsAcceleration;//アクセルが押されているかどうか

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
	


};
