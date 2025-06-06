// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Beam.h"
#include "DataStruct.h"
#include "SpaceFighter.generated.h"

UCLASS()
class ACHILESS_API ASpaceFighter : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ASpaceFighter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;



public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	void UpdateAchiLessRotation(const FRotator TargetRotation);

	UFUNCTION(BlueprintCallable)
	UWorld* GetWorldContext() { return GetWorld(); }

	//加速
	UFUNCTION(BlueprintCallable)
	void Accelerate(float Value);

	//自動減速
	UFUNCTION(BlueprintCallable)
	void AcceleReleased();

	virtual void Beam();

	UFUNCTION(BlueprintCallable, Category = "SpaceFighter")
	void TakeDamage(float InDamage);


	//ビーム連射開始処理
	UFUNCTION(BlueprintCallable)
	void StartBeam();
	//ビーム連射終了処理
	UFUNCTION(BlueprintCallable)
	void StopBeam();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ABeam> BeamClass;



	//ルートのためのコンポーネント
	UPROPERTY(EditAnywhere)
	TObjectPtr<USceneComponent> DefaultSceneRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> FighterMesh;

	
	UFUNCTION(BlueprintCallable)
	FVector GetCurrentVelocity();

	UFUNCTION(BlueprintCallable)
	void HealHP(float Heal);

	// privateでもブルプリから読み取りのみ可
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FDataStruct MyParameter;

protected:
	FVector Velocity;//移動方向のベクトル

	UPROPERTY(EditAnywhere)
	bool bIsAcceleration;//アクセルが押されているかどうか

	

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float CurrentSpeed;

	//ビーム連射用タイマーハンドル
	FTimerHandle BeamTimerHandle;

	//現在のHP
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float CurrentHp;


	float DTime;

	

};
