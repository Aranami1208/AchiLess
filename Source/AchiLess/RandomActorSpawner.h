// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RandomActorSpawner.generated.h"

//�v���g�^�C�v�錾
class UBoxComponent;

UCLASS()
class ACHILESS_API ARandomActorSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARandomActorSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void SpawnActorInBox();

public:	
	//��������I�u�W�F�N�g
	UPROPERTY(EditAnywhere, Category = "Spawner")
	TSubclassOf<AActor> SpawnObject;

	//���������
	UPROPERTY(EditAnywhere, Category = "Spawner")
	int SpawnCount = 10;

	//��������͈�
	UPROPERTY(EditAnywhere, Category = "Spawner")
	TObjectPtr<UBoxComponent> SpawnArea;

};
