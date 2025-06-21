// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SpaceFighter.h"
#include "HopeLight.generated.h"

/**
 * 
 */
UCLASS()
class ACHILESS_API AHopeLight : public ASpaceFighter
{
	GENERATED_BODY()

public:
	
	void Beam()override;

	// ゲーム開始時のみ呼ばれるファイル
	virtual void BeginPlay() override;

	// 毎フレーム呼ばれる（更新用）
	virtual void Tick(float DeltaTime) override;
	
	//ダウン中かどうか
	bool bIsDown = false;

};
