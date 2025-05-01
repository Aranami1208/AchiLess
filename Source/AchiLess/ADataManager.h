// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Dom/JsonObject.h"
#include "DataStruct.h"
#include "ADataManager.generated.h"

/**
 * 
 */
UCLASS()
class ACHILESS_API UADataManager : public UObject
{
	GENERATED_BODY()
	
	

public:
	static bool WriteJsonData(FString Name, FDataStruct& DataS);

	static bool ReadJsonData(FString& Name, FDataStruct& DataS);
};
