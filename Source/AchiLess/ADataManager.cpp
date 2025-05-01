// Fill out your copyright notice in the Description page of Project Settings.


#include "ADataManager.h"
#include "JsonUtilities.h"
#include "Runtime/Json/Public/Serialization/JsonReader.h"
#include "Misc/DateTime.h"

static const FString RootName("InventoryObject");
static const FString WritePath(FPaths::ProjectSavedDir() / "json");
static const FString FileName("JsonData.json");
static const FString FilePathFull(WritePath / FileName);

bool UADataManager::WriteJsonData(FString Name, FDataStruct& DataS)
{

	return false;
}

bool UADataManager::ReadJsonData(FString& Name, FDataStruct& DataS)
{
	FString Data;

	//データの読み込みに失敗した場合処理終了
	if (!FFileHelper::LoadFileToString(Data, *FilePathFull))return false;

	TSharedPtr<FJsonObject> JsonRootObject = MakeShareable(new FJsonObject());

	//Jsonを読み込むためのリーダーを作成
	TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory <>::Create(Data);

	if (!FJsonSerializer::Deserialize(JsonReader, JsonRootObject))return false;

	//Jsonの文字列からJsonオブジェクトによみこみ
	Name = JsonRootObject->GetStringField("Name");

	DataS.Accelerate = JsonRootObject->GetNumberField("Accelerate");
	DataS.AirFriction = JsonRootObject->GetNumberField("AirFriction");
	DataS.MaxRotationSpeed = JsonRootObject->GetNumberField("MaxRotationSpeed");
	DataS.MaxSpeed = JsonRootObject->GetNumberField("MaxSpeed");
	DataS.MinSpeed = JsonRootObject->GetNumberField("MinSpeed");
	DataS.TurnSpeed = JsonRootObject->GetNumberField("TurnSpeed");

	return true;
	
}
