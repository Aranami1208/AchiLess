#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "CardData.generated.h"

USTRUCT(BlueprintType)
struct FCardData : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 CardID;

    //カード名
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText Name;

    //フレーバーテキスト
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText FravorText;

    // カード画像
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSoftObjectPtr<UTexture2D> CardImage;

    // 使用するスキルのクラス
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<class UBaseCardSkill> CardSkillClass;

    // タグ
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName Tag;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Rarity;

    UPROPERTY(EditAnywhere,BlueprintReadWrite)
    float CoolTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float SkillValue1;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float SkillValue2;

    UPROPERTY(EditAnywhere,BlueprintReadWrite)
    bool IsValid;
};
