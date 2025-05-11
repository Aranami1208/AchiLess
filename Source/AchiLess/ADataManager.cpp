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

	//�f�[�^�̓ǂݍ��݂Ɏ��s�����ꍇ�����I��
	if (!FFileHelper::LoadFileToString(Data, *(WritePath/Name)))
	{
		UE_DEBUG_BREAK();
		return false;
	}

	TSharedPtr<FJsonObject> JsonRootObject = MakeShareable(new FJsonObject());

	//Json��ǂݍ��ނ��߂̃��[�_�[���쐬
	TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory <>::Create(Data);

	if (!FJsonSerializer::Deserialize(JsonReader, JsonRootObject))
	{
		UE_DEBUG_BREAK();
		return false;
	}

	//Json�̕����񂩂�Json�I�u�W�F�N�g�ɂ�݂���
	Name = JsonRootObject->GetStringField("Name");

	DataS.Accelerate = JsonRootObject->GetNumberField("Accelerate");
	DataS.AirFriction = JsonRootObject->GetNumberField("AirFriction");
	DataS.MaxRotationSpeed = JsonRootObject->GetNumberField("MaxRotationSpeed");
	DataS.MaxSpeed = JsonRootObject->GetNumberField("MaxSpeed");
	DataS.MaxSpeed = DataS.MaxSpeed / 0.036;//�P�ʂ�km/h����cm/s�֕ϊ�
	DataS.MinSpeed = JsonRootObject->GetNumberField("MinSpeed");
	DataS.MinSpeed = DataS.MinSpeed / 0.036;//�P�ʂ�km/h����cm/s�֕ϊ�
	DataS.TurnSpeed = JsonRootObject->GetNumberField("TurnSpeed");
	DataS.MeshFileName = JsonRootObject->GetStringField("Mesh");

	UE_DEBUG_BREAK();
	return true;
	
}
