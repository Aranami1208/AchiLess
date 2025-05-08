// Fill out your copyright notice in the Description page of Project Settings.


#include "RandomActorSpawner.h"
#include "Engine/World.h"
#include "Engine/TriggerBox.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
// Sets default values
ARandomActorSpawner::ARandomActorSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ARandomActorSpawner::BeginPlay()
{
	Super::BeginPlay();

	//�A�N�^�𐶐�
	SpawnActorInBox();
}

void ARandomActorSpawner::SpawnActorInBox()
{
	if (!SpawnObject)
	{
		UKismetSystemLibrary::PrintString(this, "�I�u�W�F�N�g��ݒ肵�Ă�������");
		return;
	}
	if (!SpawnArea)
	{
		UKismetSystemLibrary::PrintString(this, "�͈͂��w�肵�Ă�������");
		return;
	}
	if (SpawnCount < 1)
	{
		UKismetSystemLibrary::PrintString(this, "�L���Ȓl��ݒ肵�Ă�������");
		return;
	}

	UWorld* World = GetWorld();
	if (!World)return;

	//Origin = ���_�AExtent = �͈�
	FVector Orign, Extent;

	//�ݒ肳�ꂽTriggerBox�̃T�C�Y���擾
	SpawnArea->GetActorBounds(false, Orign, Extent);

	for (int i = 0; i < SpawnCount; i++)
	{
		//�w�肵���͈͂��烉���_���Ɉ�_���擾
		FVector SpawnPoint = UKismetMathLibrary::RandomPointInBoundingBox(Orign,Extent);

		//�w�肳�ꂽ���W��Transform�ɕϊ�
		FTransform SpawnTransform(FRotator::ZeroRotator, SpawnPoint);

		//�w�肳�ꂽ���W�ɃA�N�^�[�𐶐�
		TObjectPtr<AActor> SpawnedActor = World->SpawnActor<AActor>(SpawnObject, SpawnTransform);
	}
}


