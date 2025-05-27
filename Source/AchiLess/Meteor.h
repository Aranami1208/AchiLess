// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DestructibleActor.h"
#include "Meteor.generated.h"

UCLASS()
class ACHILESS_API AMeteor : public ADestructibleActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMeteor();

	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	

	UPROPERTY(EditAnywhere)
	UMaterialInterface* BaseMaterial = LoadObject<UMaterialInterface>(nullptr,TEXT("/Game/Assets/Models/Meteor/Texture/BaseMaterial.BaseMaterial"));

	UPROPERTY(EditAnywhere)
	FVector MinScale = FVector(1.0f);

	UPROPERTY(EditANywhere)
	FVector MaxScale = FVector(10.0f);
};
