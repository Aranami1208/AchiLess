// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Beam.generated.h"

UCLASS()
class ACHILESS_API ABeam : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABeam();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void DestroyMe();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//移動方向を設定
	void SetVelocity(FVector Velo);

	//移動速度
	FVector Velocity;

};
