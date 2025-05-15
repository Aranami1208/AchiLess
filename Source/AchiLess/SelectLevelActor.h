// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DataStruct.h"
#include "SelectLevelActor.generated.h"


UCLASS()
class ACHILESS_API ASelectLevelActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASelectLevelActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void LoadAllJson();

	//�Z���Nă��x���̃E�B�W�F�b�g�𐶐�
	void CreateSelectLevelWidget();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	//AchiLess�̃p�����[�^
	TArray<FDataStruct> AchiLessParams;

};
