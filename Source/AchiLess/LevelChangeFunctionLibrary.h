#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "LevelChangeFunctionLibrary.generated.h"

UCLASS()
class ACHILESS_API ULevelChangeFunctionLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:

    UFUNCTION(BlueprintCallable, Category = "Level")
    static void LevelChanger(UObject* WorldContextObject, FName LevelName);
};
