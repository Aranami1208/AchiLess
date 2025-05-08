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

	UPROPERTY(BlueprintReadOnly)
	FDataStruct parameter;

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
	UFUNCTION(BlueprintCallable)
	void Accelerate(float Value);//加速

	UFUNCTION(BlueprintCallable)
	void AcceleReleased();


private:
	
	FVector Velocity;//移動方向のベクトル

	
	
	float MaxRotationSpeed;

	// privateでもブルプリから読み取りのみ可
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float CurrentSpeed;//現在のスピード

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float MaxSpeed;//最大スピード

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float MiniSpeed;//最低スピード

	
	UPROPERTY(EditAnywhere)
	bool bIsAcceleration;//アクセルが押されているかどうか

	
	
	
	UPROPERTY(VisibleAnywhere)
	class USpringArmComponent* CameraSpringArm;

	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* Camera;
	


};
