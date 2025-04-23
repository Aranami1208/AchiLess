// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Class_AchiLess.generated.h"

UCLASS()
class ACHILESS_API AClass_AchiLess : public APawn
{
	GENERATED_BODY()

public:
	//�R���X�g���N�^
	AClass_AchiLess();

protected:
	// �Q�[���J�n���̂݌Ă΂��t�@�C��
	virtual void BeginPlay() override;

public:
	// ���t���[���Ă΂��i�X�V�p�j
	virtual void Tick(float DeltaTime) override;

	// ���͌n�R���|�[�l���g
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	//���͊֐�
	void Pitch(float Value);
	void Yaw(float Value);
	void Roll(float Value);

	void Accelerate(float Value);//����

	FVector Velocity;//�ړ������̃x�N�g��

	float CurrentSpeed;//���݂̃X�s�[�h
	float MaxSpeed;//�ő�X�s�[�h


	//�����p�p�����[�^
	UPROPERTY(EditAnywhere)

	float Acceleration = 1000.f;//�����̋���

	UPROPERTY(EditAnywhere)
	float TurnSpeed = 50.f;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* AchilessMesh;

	
	UPROPERTY(VisibleAnywhere)
	class USpringArmComponent* CameraSpringArm;

	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* Camera;
	


};
