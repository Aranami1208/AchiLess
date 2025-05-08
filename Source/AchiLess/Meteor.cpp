// Fill out your copyright notice in the Description page of Project Settings.


#include "Meteor.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Kismet/KismetSystemLibrary.h"
// Sets default values
AMeteor::AMeteor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	//メッシコンポーネントをルートコンポーネントにする
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	RootComponent = MeshComponent;
}

// Called when the game starts or when spawned
void AMeteor::BeginPlay()
{
	Super::BeginPlay();

	//スケールを一定範囲からランダムに決定()
	FVector Scale = FMath::RandPointInBox(FBox(MinScale, MaxScale));
	SetActorScale3D(Scale);

	FString TextureFolderPath = TEXT("/Game/Assets/Models/Meteor/Texture");

	//AssetRegistryからテクスチャを取得
	FAssetRegistryModule AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");

	TArray<FAssetData> AssetDataList;
	//アセットレジストリのフィルター
	FARFilter Filter;
	FName ClassName = (FName)(UTexture::StaticClass()->GetName());
	Filter.ClassNames.Add(ClassName);
	Filter.PackagePaths.Add(*TextureFolderPath);
	Filter.bRecursivePaths = true;

	//フィルターで絞ったアセットをリストに渡す
	AssetRegistryModule.Get().GetAssets(Filter, AssetDataList);

	//テクスチャがなかった場合は処理しない
	if (AssetDataList.Num() <= 0)
	{
		UKismetSystemLibrary::PrintString(this, "NoTexture");
		return;
	}

	//リストの中からランダムに一つ選出
	int Index = FMath::RandRange(0, AssetDataList.Num() - 1);

	UObject* LoadedObject = AssetDataList[Index].GetAsset();

	if (UTexture* Texture = Cast<UTexture>(LoadedObject))
	{
		if (!BaseMaterial)return;

		UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(BaseMaterial, this);
		DynamicMaterial->SetTextureParameterValue("BaseMaterial", Texture);
		MeshComponent->SetMaterial(0, DynamicMaterial);
	}
	else
	{
		UKismetSystemLibrary::PrintString(this, "キャストに失敗しました");
	}
}