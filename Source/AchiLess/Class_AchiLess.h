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

private:
	//入力関数
	void Pitch(float Value);
	void Yaw(float Value);
	void Roll(float Value);

	void Accelerate(float Value);//加速

	FVector Velocity;//移動方向のベクトル

	float CurrentSpeed;//現在のスピード
	float MaxSpeed;//最大スピード


	//調整用パラメータ
	UPROPERTY(EditAnywhere)

	float Acceleration = 1000.f;//加速の強さ

	UPROPERTY(EditAnywhere)
	float TurnSpeed = 50.f;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* AchilessMesh;

	
	UPROPERTY(VisibleAnywhere)
	class USpringArmComponent* CameraSpringArm;

	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* Camera;
	


};
