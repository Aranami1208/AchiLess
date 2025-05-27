// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DestructibleActor.generated.h"

UCLASS()
class ACHILESS_API ADestructibleActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADestructibleActor();

	UFUNCTION(BlueprintCallable)
	virtual void TakeDamage(float Damage);

	UFUNCTION(BlueprintCallable)
	int32 GetCurrentHP();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	

	float CurrentHp;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
