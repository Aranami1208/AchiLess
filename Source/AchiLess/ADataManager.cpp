// Fill out your copyright notice in the Description page of Project Settings.


#include "ADataManager.h"
#include "JsonUtilities.h"
#include "Runtime/Json/Public/Serialization/JsonReader.h"
#include "Misc/DateTime.h"

static const FString RootName("InventoryObject");
static const FString WritePath(FPaths::ProjectContentDir() / "Json");

bool UADataManager::WriteJsonData(FString Name, FDataStruct& DataS)
{

	return false;
}

bool UADataManager::ReadJsonData(FString Name, FDataStruct& DataS)
{
	FString Data;

	//データの読み込みに失敗した場合処理終了
	if (!FFileHelper::LoadFileToString(Data, *(WritePath/Name)))
	{
		UE_DEBUG_BREAK();
		return false;
	}

	TSharedPtr<FJsonObject> JsonRootObject = MakeShareable(new FJsonObject());

	//Jsonを読み込むためのリーダーを作成
	TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory <>::Create(Data);

	if (!FJsonSerializer::Deserialize(JsonReader, JsonRootObject))
	{
		UE_DEBUG_BREAK();
		return false;
	}

	//Jsonの文字列からJsonオブジェクトによみこみ
	Name = JsonRootObject->GetStringField("Name");

	DataS.Accelerate = JsonRootObject->GetNumberField("Accelerate");
	DataS.AirFriction = JsonRootObject->GetNumberField("AirFriction");
	DataS.MaxRotationSpeed = JsonRootObject->GetNumberField("MaxRotationSpeed");
	DataS.MaxSpeed = JsonRootObject->GetNumberField("MaxSpeed");
	DataS.MaxSpeed = DataS.MaxSpeed / 0.036;//単位をkm/hからcm/sへ変換
	DataS.MinSpeed = JsonRootObject->GetNumberField("MinSpeed");
	DataS.MinSpeed = DataS.MinSpeed / 0.036;//単位をkm/hからcm/sへ変換
	DataS.TurnSpeed = JsonRootObject->GetNumberField("TurnSpeed");
	DataS.MeshFileName = JsonRootObject->GetStringField("Mesh");

	UE_DEBUG_BREAK();
	return true;
	
}
