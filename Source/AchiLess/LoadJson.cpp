// Fill out your copyright notice in the Description page of Project Settings.


#include "LoadJson.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"

LoadJson::LoadJson(FString Filepath)
{
	FString JsonFilePath = FPaths::ProjectContentDir() / Filepath;//�t�@�C���p�X
	FString JsonString;//�t�@�C�����̕���

	//�t�@�C�����ǂݍ��߂Ȃ������珈�����Ȃ�
	if (!FFileHelper::LoadFileToString(JsonString, *JsonFilePath))return;

	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);


}

LoadJson::~LoadJson()
{
}
