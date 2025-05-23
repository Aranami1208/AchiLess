// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DataStruct.h"
#include "CardItemWidget.h"
#include "SelectLevelActor.generated.h"


UCLASS()
class ACHILESS_API ASelectLevelActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASelectLevelActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void LoadAllJson();

	//セレクﾄレベルのウィジェットを生成
	void CreateSelectLevelWidget();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void AchiLessSelectLeft();
	UFUNCTION(BlueprintCallable)
	void AchiLessSelectRight();

public:

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	//AchiLessのパラメータ
	TArray<FDataStruct> AchiLessParams;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int SelectIndex = 0;

	//ファイルの個数
	int FileNum;

	UPROPERTY(EditAnyWhere,BlueprintReadWrite)
	TSubclassOf<UCardItemWidget> CardItemClass;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	class UUniformGridPanel* GridPanel = nullptr;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TArray<UCardItemWidget*> DeckCard;

	UPROPERTY()
	TArray<UCardItemWidget*> GenaratedWidgets;
};
