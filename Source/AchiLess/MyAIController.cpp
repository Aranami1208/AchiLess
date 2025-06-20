#include "MyAIController.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Class_AchiLess.h"
#include "BehaviorTree/BlackboardComponent.h" 

AMyAIController::AMyAIController()
{
    CurrentPathIndex = 0;
}

void AMyAIController::BeginPlay()
{
    Super::BeginPlay();

    // ワールドからPathfindingSubsystemを取得
    if (UWorld* World = GetWorld())
    {
        PathfindingSubsystem = World->GetSubsystem<UPathfindingSubsystem>();
        if (!PathfindingSubsystem)
        {
            UE_LOG(LogTemp, Error, TEXT("PathfindingSubsystem not found! Make sure it's set up as a UWorldSubsystem."));
        }
    }
}

void AMyAIController::RequestPathToLocation(const FVector& TargetLocation)
{
    UWorld* World = GetWorld();
    if (!World) return;

    UPathfindingSubsystem* PathSubsystem = World->GetSubsystem<UPathfindingSubsystem>();
    if (PathSubsystem && GetPawn())
    {
        // コールバック用のデリゲートを作成し、このクラスの OnPathfindingCompleted 関数をバインドする
        FOnPathfindingComplete Callback;
        Callback.BindUFunction(this, FName("OnPathfindingCompleted"));

        // 非同期に経路探索を開始
        PathSubsystem->FindPathAsync(GetPawn()->GetActorLocation(), TargetLocation, Callback);

        UE_LOG(LogTemp, Log, TEXT("Pathfinding request sent asynchronously."));
    }
    return;
}

void AMyAIController::OnPathFindingCompleted(const TArray<FVector>& Path)
{
    UBlackboardComponent* BlackboardComp = GetBlackboardComponent();
    if (!BlackboardComp)return;

    if (Path.Num() > 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("Async pathfinding complete! Path has %d points."), Path.Num());

        CurrentPath = Path;
        CurrentPathIndex = 0;
        UKismetSystemLibrary::PrintString(this, "ResetPathIndex");

        int32 Index = 0;

        FVector Start = Path[0];
        FVector End = Start;

        
        for (FVector Path : CurrentPath)
        {
            End = Path;
            // 赤色の矢印を描画
            UKismetSystemLibrary::DrawDebugArrow(
                GetWorld(),          // ワールド
                Start,       // 開始点
                End,         // 終了点
                1000.0f,              // 矢じりのサイズ
                FColor::Green,         // 色
                0,               // 描画時間 (0か-1なら1フレーム)
                400.0f                // 線の太さ
            );
            /*
            if (Index != CurrentPath.Num() - 1)
                DrawDebugBox(GetWorld(), Path, FVector(2000.f ), FColor::Green, true, 5.0f, 0, 700.0f);
            else
                DrawDebugBox(GetWorld(), Path, FVector(2000.f), FColor::Magenta, true, 5.0f, 0, 700.0f);
            */

            Index++;

            Start = Path;
        }

        // ブラックボードに「パス有り」と設定
        BlackboardComp->SetValueAsBool(FName("HasPath"), true);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Async pathfinding failed to find a path."));
        CurrentPath.Empty();
        CurrentPathIndex = -1;

        // ブラックボードに「パス無し」と設定
        BlackboardComp->SetValueAsBool(FName("HasPath"), false);
    }
}

FRotator AMyAIController::GetNextPathPointRotation(float DeltaTime, float AcceptanceRadius, float& ToNextPointDistance)
{
    UKismetSystemLibrary::PrintString(this, FString::FromInt(CurrentPathIndex));
    if (CurrentPath.Num() == 0 || CurrentPathIndex >= CurrentPath.Num() || !GetPawn())
    {
        return FRotator::ZeroRotator; // 有効なパスがない、または終了している
    }

    FVector CurrentLocation = GetPawn()->GetActorLocation();
    FVector TargetPoint = CurrentPath[CurrentPathIndex];
    
    ToNextPointDistance = FVector::Distance(CurrentLocation, TargetPoint);

    // ターゲットポイントに十分に近づいたら次のポイントへ
    if (ToNextPointDistance < AcceptanceRadius)
    {
        AdvanceToNextPathPoint();
        if (CurrentPathIndex >= CurrentPath.Num())
        {
            // パス終了
            return FRotator::ZeroRotator;
        }
        TargetPoint = CurrentPath[CurrentPathIndex]; // 新しいターゲットポイントをセット
    }

    FVector DirectionToTarget = (TargetPoint - CurrentLocation).GetSafeNormal();
    return DirectionToTarget.Rotation();
}

void AMyAIController::AdvanceToNextPathPoint()
{
    CurrentPathIndex++;
    if (CurrentPathIndex >= CurrentPath.Num())
    {
        CurrentPath.Empty(); // パスをクリア
        CurrentPathIndex = 0;
    }
}

bool AMyAIController::HasValidPath() const
{
    return CurrentPath.Num() > CurrentPathIndex;
}

FVector AMyAIController::GetCurrentTargetPathPoint() const
{
    if (HasValidPath())
    {
        return CurrentPath[CurrentPathIndex];
    }
    return FVector::ZeroVector;
}