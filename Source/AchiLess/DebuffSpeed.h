// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DebuffSpeed.generated.h"

UCLASS()
class ACHILESS_API ADebuffSpeed : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADebuffSpeed();
	class AClass_AchiLess* User;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Detection")
	float DefaultDetectionRadius; // 検出半径

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Detection")
	float DefaultMaxDetectionAngle; // 検出角度（半角、度数）
	
	class AClass_AchiLess* TargetActor;
	

	//効果を消す
	FTimerHandle RestoreTimer;

	void RestoreEffect();
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	float EffectRate;
	float EffectTime;

};
