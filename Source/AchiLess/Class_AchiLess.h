// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "SpaceFighter.h"
#include "DataStruct.h"
#include "CardSkillWidget.h"
#include "Sound/SoundBase.h"
#include "Class_AchiLess.generated.h"

UCLASS()
class ACHILESS_API AClass_AchiLess : public ASpaceFighter
{
	GENERATED_BODY()

public:
	//コンストラクタ
	AClass_AchiLess();

	void Beam()override;

protected:
	// ゲーム開始時のみ呼ばれるファイル
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UCardSkillWidget> CardSkillWidgetClass;

public:


	// 毎フレーム呼ばれる（更新用）
	virtual void Tick(float DeltaTime) override;

	// 入力系コンポーネント
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	UFUNCTION(BlueprintCallable)
	void InitDeck();

	UFUNCTION(BlueprintCallable)
	void Pitch(float Value);
	UFUNCTION(BlueprintCallable)
	void Yaw(float Value);
	UFUNCTION(BlueprintCallable)
	void Roll(float Value);

	
	UFUNCTION(BlueprintCallable)
	void SelectSkill1();
	UFUNCTION(BlueprintCallable)
	void SelectSkill2();
	UFUNCTION(BlueprintCallable)
	void SelectSkill3();
	UFUNCTION(BlueprintCallable)
	void SelectSkill4();

	//使用するスキルのインデックス
	int32 SkillIndex = -1;

	UFUNCTION(BlueprintCallable)
	void ExecuteSkill();

	UFUNCTION(BlueprintCallable)
	void ChangeDeck();

	UFUNCTION(BlueprintCallable)
	void ReChangeDeck();

	UFUNCTION(BlueprintCallable)
	class AActor* SpawnSkillActor(TSubclassOf<AActor> SpawnActorClass);
	
	//使用するスキルのインデックスに加算する値
	int32 UseDeck = 0;

	//ブースト
	UFUNCTION(BlueprintCallable)
	void Boost(float Seconds);
	UFUNCTION(BlueprintCallable)
	void BoostReleased();

	float CameraPitchSpeed = 45.0f;
	float CamerayawSpeed = 90.0f;

	//デッキの情報
	UPROPERTY(EditAnywhere, EditFixedSize, BlueprintReadWrite)
	TArray<FCardData> DeckData;
	UPROPERTY(EditAnywhere, EditFixedSize, BlueprintReadWrite)
	TArray<class UBaseCardSkill*> CardSkills;
	UPROPERTY(EditAnywhere,EditFixedSize,BlueprintReadWrite)
	TArray<UCardSkillWidget*> SkillWidgets;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FCardData EmptyData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FCardData SelectCardData;

	UFUNCTION(BlueprintCallable)
	UWorld* GetWorldCntext();

	UPROPERTY(EditAnywhere, Category = "Sound")
	USoundBase* HealSound;

	UFUNCTION(BlueprintCallable)

	void PlaySoundEffect(USoundBase* InSound);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsInvencible = false;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float SpeedBuffRate = 1.0f;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float AttackBuffRate = 1.0f;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float DiffenceBuffRate = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DamageCutRate = 0.0f;

	void TakeDamage(float InDamage);

	

private:
	

	UPROPERTY(EditAnywhere)
	FString AchilessName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float CurrentBoost;
	
	

	//ブーストの無敵時間
	UPROPERTY(EditAnywhere)
	float InvincibleSec;

	UPROPERTY(EditAnywhere)
	float InvincibleCount;

	

	//加速率
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float BoostRate = 1;

	UPROPERTY(EditAnywhere)
	float BoostCost = 2;

	//ブーストを使い切ったときにはロックを掛ける
	UPROPERTY(EditAnywhere)
	bool BoostLock = false;

	UPROPERTY(EditAnywhere)
	bool bIsBoosting = false;
	
	
	UPROPERTY(VisibleAnywhere)
	class USpringArmComponent* CameraSpringArm;

	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* Camera;
	
	float CurrentMouseYInput;
	float CurrentMouseXInput;

	//AIがコントロールしている
	UPROPERTY(EditAnywhere,BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool bIsAIControll;

	//HUDの円のサイズ（px）
	float HUDCircleRadiusPixel = 180.0f;
	
	//ロックオン対象
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<ASpaceFighter> LockOnTargetFigter;

	//矢印の対象
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	AActor* ArrowTarget;

	float MaxLockOnDistance;

	//視野角
	float LockOnFOV;

	//ロックオンチェックの間隔
	float LockOnCheckInterval;

	//定期的に処理するためのタイマーハンドル
	FTimerHandle LockOnCheckTimerHandle;

	//HUDの円の中心の画面座標を求める
	FVector2D GetHUDCircleCenterLocation();

	//ターゲティングしているかチェックする関数
	void CheckOnTarget();

	//PlayerController
	APlayerController* PC;

	
	
	

};
