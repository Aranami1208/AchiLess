// Fill out your copyright notice in the Description page of Project Settings.


#include "RandomActorSpawner.h"
#include "Engine/World.h"
#include "Engine/TriggerBox.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
// Sets default values
ARandomActorSpawner::ARandomActorSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	//�{�b�N�X�R���|�[�l���g�̐���
	SpawnArea = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnArea"));
	//���[�g�R���|�[�l���g�ɐݒ�
	RootComponent = SpawnArea;
	//�����T�C�Y�ݒ�
	SpawnArea->SetBoxExtent(FVector(100.0f, 100.0f, 100.0f));
	//�R���W������ݒ肵�Ȃ�
	SpawnArea->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//�Q�[�����͕\�����Ȃ�
	SpawnArea->bHiddenInGame = true;
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
	if (SpawnCount < 1)
	{
		UKismetSystemLibrary::PrintString(this, "�L���Ȓl��ݒ肵�Ă�������");
		return;
	}

	

	UWorld* World = GetWorld();
	if (!World)return;

	//Origin = ���_�AExtent = �͈�
	FVector Origin, Extent;

	//���_�擾
	Origin = SpawnArea->GetComponentLocation();
	//�{�b�N�X�̃T�C�Y���擾
	Extent = SpawnArea->GetScaledBoxExtent();

	for (int i = 0; i < SpawnCount; i++)
	{
		//�w�肵���͈͂��烉���_���Ɉ�_���擾
		FVector SpawnPoint = UKismetMathLibrary::RandomPointInBoundingBox(Origin,Extent);

		//�w�肳�ꂽ���W��Transform�ɕϊ�
		FTransform SpawnTransform(FRotator::ZeroRotator, SpawnPoint);

		//�w�肳�ꂽ���W�ɃA�N�^�[�𐶐�
		TObjectPtr<AActor> SpawnedActor = World->SpawnActor<AActor>(SpawnObject, SpawnTransform);
	}
}


