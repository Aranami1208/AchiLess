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

	UPROPERTY(BlueprintReadOnly)
	FDataStruct parameter;

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
	UFUNCTION(BlueprintCallable)
	void Accelerate(float Value);//����

	UFUNCTION(BlueprintCallable)
	void AcceleReleased();


private:
	
	FVector Velocity;//�ړ������̃x�N�g��

	
	
	float MaxRotationSpeed;

	// private�ł��u���v������ǂݎ��̂݉�
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float CurrentSpeed;//���݂̃X�s�[�h

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float MaxSpeed;//�ő�X�s�[�h

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float MiniSpeed;//�Œ�X�s�[�h

	
	UPROPERTY(EditAnywhere)
	bool bIsAcceleration;//�A�N�Z����������Ă��邩�ǂ���

	
	
	
	UPROPERTY(VisibleAnywhere)
	class USpringArmComponent* CameraSpringArm;

	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* Camera;
	


};
