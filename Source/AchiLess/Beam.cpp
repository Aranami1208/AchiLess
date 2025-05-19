// Fill out your copyright notice in the Description page of Project Settings.


#include "Beam.h"

// Sets default values
ABeam::ABeam()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABeam::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABeam::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector Forward = GetActorForwardVector();//�i�s�����x�N�g�����擾����
	Velocity = Forward * 100000;

	//�ړ��ƏՓ˔��菈�� 
	AddActorWorldOffset(Velocity * DeltaTime, true);

}

void ABeam::SetVelocity(FVector Velo)
{
	//�����x�N�g���𐳋K��
	FVector Forward = Velo.GetSafeNormal();

	SetActorRotation(Forward.Rotation());
}

