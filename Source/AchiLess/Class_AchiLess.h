// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "DataStruct.h"
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

	//���[�g�̂��߂̃R���|�[�l���g
	UPROPERTY(EditAnywhere)
	TObjectPtr<USceneComponent> DefaultSceneRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> AchilessMesh;

	// ���t���[���Ă΂��i�X�V�p�j
	virtual void Tick(float DeltaTime) override;

	// ���͌n�R���|�[�l���g
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	//���͊֐��u���v������Ăяo���\

	UFUNCTION(BlueprintCallable)
	void Pitch(float Value);
	UFUNCTION(BlueprintCallable)
	void Yaw(float Value);
	UFUNCTION(BlueprintCallable)
	void Roll(float Value);


	//����
	UFUNCTION(BlueprintCallable)
	void Accelerate(float Value);

	//��������
	UFUNCTION(BlueprintCallable)
	void AcceleReleased();

	//�u�[�X�g
	UFUNCTION(BlueprintCallable)
	void Boost(float Seconds);
	UFUNCTION(BlueprintCallable)
	void BoostReleased();
	
private:
	
	FVector Velocity;//�ړ������̃x�N�g��

	float MaxRotationSpeed;

	UPROPERTY(EditAnywhere)
	FString AchilessName;

	// private�ł��u���v������ǂݎ��̂݉�
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FDataStruct MyParameter;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float CurrentSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float CurrentBoost;
	
	UPROPERTY(EditAnywhere)
	bool bIsAcceleration;//�A�N�Z����������Ă��邩�ǂ���

	//�u�[�X�g�̖��G����
	UPROPERTY(EditAnywhere)
	float InvincibleSec;

	//������
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float BoostRate = 1;

	UPROPERTY(EditAnywhere)
	float BoostCost = 2;

	//�u�[�X�g���g���؂����Ƃ��ɂ̓��b�N���|����
	UPROPERTY(EditAnywhere)
	bool BoostLock = false;

	UPROPERTY(EditAnywhere)
	bool bIsBoosting = false;
	
	
	UPROPERTY(VisibleAnywhere)
	class USpringArmComponent* CameraSpringArm;

	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* Camera;
	


};
