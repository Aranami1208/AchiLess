// Fill out your copyright notice in the Description page of Project Settings.
#include "Class_AchiLess.h"

//�X�v�����O�A�[���i�J�����p�̃R���|�[�l���g�j
#include "GameFramework/SpringArmComponent.h"

//�J�����̃R���|�[�l���g
#include "Camera/CameraComponent.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"


#include "ADataManager.h" 

#include "CharacterData.h"

// Sets default values
AClass_AchiLess::AClass_AchiLess() :
	AchilessMesh(nullptr),
	CameraSpringArm(nullptr),
	Camera(nullptr),
	bIsAcceleration(false)
{


	//���t���[��Tick()���Ăԏ���
	PrimaryActorTick.bCanEverTick = true;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));

	AchilessMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AchiLessMesh"));
	RootComponent = DefaultSceneRoot;//���[�g�R���|�[�l���g�ɐݒ�


	//SpringArm�̐ݒ�
	//�X�v�����O�A�[���R���|�[�l���g�̐���
	CameraSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraSpringArm"));

	//bUseControllerRotationYaw = true;

	CameraSpringArm->SetupAttachment(RootComponent);//���[�g�R���|�[�l���g�ɃA�^�b�`�H
	CameraSpringArm->TargetArmLength = 2000.f;//�Ώۂ̃I�u�W�F�N�g����̋��� 
	CameraSpringArm->SetRelativeLocation(FVector(0.f, 0.f, 1000.f));//�f�t�H���g�̃J�����̈ʒu
	CameraSpringArm->SetRelativeRotation(FRotator(-10.f, 0.f, 0.f));//�f�t�H���g�̃J�����̃��[�e�[�V����
	//�X�v�����O�A�[���̉�]���ݒ�
	CameraSpringArm->bInheritPitch = true;
	CameraSpringArm->bInheritYaw = true;
	CameraSpringArm->bInheritRoll = true;
	//CameraSpringArm->bUsePawnControlRotation = true;


	//�J�����R���|�[�l���g�̐���
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));

	Camera->SetupAttachment(CameraSpringArm);//�X�v�����O�A�[���ɃJ�������A�^�b�`
	

	AutoPossessPlayer = EAutoReceiveInput::Player0;  // Player0�Ɏ����ő����n��

	AchilessMesh->SetupAttachment(DefaultSceneRoot);
	
}
// Called when the game starts or when spawned
void AClass_AchiLess::BeginPlay()
{
	Super::BeginPlay();

	Camera->FieldOfView = 90;

	//AchilessName = "TypeSpeed";

	//�f�[�^�}�l�[�W���[
	//UADataManager* DataManager = NewObject<UADataManager>();

	//DataManager->ReadJsonData(AchilessName+".json", MyParameter);

	UCharacterData* CharacterData = Cast<UCharacterData>(UGameplayStatics::GetGameInstance(GetWorld()));

	MyParameter = CharacterData->GetParameter();

	//�u�[�X�g�𖢎g�p��Ԃɂ���
	CurrentBoost = MyParameter.MaxBoost;

	FString ModelFilePath("/Game/Assets/Models/AhiLess");
	FString FullPath = (ModelFilePath / MyParameter.MeshFileName / MyParameter.MeshFileName + "." + MyParameter.MeshFileName);

	UStaticMesh* Mesh = LoadObject<UStaticMesh>(NULL, *FullPath, NULL, LOAD_None, NULL);

	

	if (!Mesh)
	{
		//���b�V�����Z�b�g�ł��Ȃ�������
		
		UE_DEBUG_BREAK();
		return;
	}

	if (!AchilessMesh->SetStaticMesh(Mesh))
	{
		UKismetSystemLibrary::PrintString(this, "Could not set mesh");
		//���b�V�����Z�b�g�ł��Ȃ�������
		UE_DEBUG_BREAK();

	}
	
	//UE_DEBUG_BREAK();
	
}

// Called every frame
void AClass_AchiLess::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector Forward = GetActorForwardVector();//�i�s�����x�N�g�����擾����
	Velocity = Forward * CurrentSpeed * BoostRate;//�X�s�[�h���|�����ړ���

	//�ړ��ƏՓ˔��菈�� 
	AddActorWorldOffset(Velocity * DeltaTime, true);

	
	if (!bIsAcceleration) {

		//�������Ă��Ȃ��Ƃ��̏���
		CurrentSpeed = FMath::Clamp(CurrentSpeed - (MyParameter.AirFriction * GetWorld()->GetDeltaSeconds()), MyParameter.MinSpeed, MyParameter.MaxSpeed);
	}

	//�u�[�X�g���Ă��Ȃ��Ƃ��̏���
	if (!bIsBoosting)
	{
		CurrentBoost = FMath::Clamp(CurrentBoost + BoostCost, 0, MyParameter.MaxBoost);
		
		//���x��ʏ펞�̏�Ԃɖ߂�
		BoostRate = 1.0;

		//�u�[�X�g�Q�[�W���}�b�N�X�o�Ȃ��Ƃ��̓X�L�b�v
		if (CurrentBoost != MyParameter.MaxBoost)return;

		//�u�[�X�g���b�N������
		if (BoostLock) BoostLock = false;
	}

}

// Called to bind functionality to input
void AClass_AchiLess::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	//PlayerInputComponent->BindAxis("Pitch", this, &AClass_AchiLess::Pitch);
	//PlayerInputComponent->BindAxis("Yaw", this, &AClass_AchiLess::Yaw);
	//PlayerInputComponent->BindAxis("Roll", this, &AClass_AchiLess::Roll);
	//PlayerInputComponent->BindAxis("Accelerate", this, &AClass_AchiLess::Accelerate);
	

}

void AClass_AchiLess::Pitch(float Value)
{
	Value = FMath::Clamp(Value,- MyParameter.MaxRotationSpeed, MyParameter.MaxRotationSpeed);
	//�s�b�`����
	AddActorLocalRotation(FRotator(Value * MyParameter.TurnSpeed * GetWorld()->GetDeltaSeconds(), 0.f, 0.f));
}

void AClass_AchiLess::Yaw(float Value)
{
	//AddActorLocalRotation(FRotator(0.f, Value * TurnSpeed * GetWorld()->GetDeltaSeconds(), 0.f));
}

void AClass_AchiLess::Roll(float Value)
{
	AddActorLocalRotation(FRotator(0.f, 0.f, Value * MyParameter.TurnSpeed * GetWorld()->GetDeltaSeconds()));
}

void AClass_AchiLess::Accelerate(float Value)
{
	//Clamp�͔͈͐���
	bIsAcceleration = true;
	CurrentSpeed = FMath::Clamp(CurrentSpeed + (Value * MyParameter.Accelerate * GetWorld()->GetDeltaSeconds()), MyParameter.MinSpeed, MyParameter.MaxSpeed);
	
}

void AClass_AchiLess::AcceleReleased()
{
	bIsAcceleration = false;
}

void AClass_AchiLess::Boost(float Seconds)
{
	//�u�[�X�g���b�N���������Ă���Ƃ��͏������Ȃ�
	if (BoostLock)return;

	//�u�[�X�g���
	bIsBoosting = true;

	CurrentBoost -= BoostCost;

	//�����A�g���؂�����u�[�X�g���������Ȃ�
	if (CurrentBoost <= 0)
	{
		CurrentBoost = 0;
		BoostLock = true;
		return;
	}

	BoostRate = 1.5f;
}

void AClass_AchiLess::BoostReleased()
{
	bIsBoosting = false;
}


