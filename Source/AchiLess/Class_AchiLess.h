// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
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
	UFUNCTION(BlueprintCallable)
	void Accelerate(float Value);//加速

	UFUNCTION(BlueprintCallable)
	void AcceleReleased();


private:
	
	FVector Velocity;//移動方向のベクトル

	float CurrentSpeed;//現在のスピード
	float MaxSpeed;//最大スピード
	float MiniSpeed;//最低スピード

	UPROPERTY(EditAnywhere)
	bool bIsAcceleration;//アクセルが押されているかどうか

	//調整用パラメータ
	UPROPERTY(EditAnywhere)

	float Acceleration = 1500.f;//加速の強さ
	float AirFriction = 500;//空気摩擦の減速量

	UPROPERTY(EditAnywhere)
	float TurnSpeed = 50.f;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* AchilessMesh;

	
	UPROPERTY(VisibleAnywhere)
	class USpringArmComponent* CameraSpringArm;

	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* Camera;
	


};
