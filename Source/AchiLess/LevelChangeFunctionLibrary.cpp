#include "LevelChangeFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Engine/Engine.h"


void ULevelChangeFunctionLibrary::LevelChanger(UObject* WorldContextObject, FName LevelName)
{
    if (!WorldContextObject) return;

    UWorld* World = GEngine->GetWorldFromContextObjectChecked(WorldContextObject);
    if (World)
    {
        UGameplayStatics::OpenLevel(WorldContextObject, LevelName);
    }
}
