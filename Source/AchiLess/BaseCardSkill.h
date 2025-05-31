// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CardData.h"
#include "BaseCardSkill.generated.h"



UCLASS()
class ACHILESS_API UBaseCardSkill : public UObject
{
	GENERATED_BODY()

public:

	//カードのデータ
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	FCardData CardData;

	TWeakObjectPtr<class AClass_AchiLess> Owner;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	class UCardSkillWidget* Widget;

	//ブループリントでオーバーライドできるように
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	void ExecuteSkill(class ASpaceFighter* Target);
	virtual void ExecuteSkill_Implementation(class ASpaceFighter* Target);//C++オーバーライドする用

	UFUNCTION(BlueprintCallable)
	void StartCoolDown();

	UFUNCTION(BlueprintCallable)
	float GetCurrentCooldown();

	UFUNCTION(BlueprintCallable)
	bool IsOnCoolDown();

	UFUNCTION(BlueprintCallable)
	void UpdateCooDown(float DeltaTime);

protected:

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	float CurrentCooldownTime;
};
