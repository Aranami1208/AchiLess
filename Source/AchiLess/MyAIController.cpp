#include "MyAIController.h"
#include "Kismet/GameplayStatics.h"
#include "Class_AchiLess.h"

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

bool AMyAIController::RequestPathToLocation(const FVector& TargetLocation)
{
    CurrentPath.Empty();
    CurrentPathIndex = 0;

    if (PathfindingSubsystem && GetPawn())
    {
        CurrentPath = PathfindingSubsystem->FindPath(GetPawn()->GetActorLocation(), TargetLocation);
        DrawDebugSphere(GetWorld(), TargetLocation, 2000.0f, 12, FColor::Green, false, 10.0f);
        if (CurrentPath.Num() > 0)
        {
            for (FVector Path : CurrentPath)
            {
                DrawDebugBox(GetWorld(), Path, FVector(5000.f), FColor::Green, true, 0.1f, 0, 700.0f);
            }
            UE_LOG(LogTemp, Log, TEXT("Path found with %d points."), CurrentPath.Num());
            return true;
        }
       
    }
    UE_LOG(LogTemp, Warning, TEXT("Failed to find path."));
    return false;
}

FRotator AMyAIController::GetNextPathPointRotation(float DeltaTime, float AcceptanceRadius)
{
    if (CurrentPath.Num() == 0 || CurrentPathIndex >= CurrentPath.Num() || !GetPawn())
    {
        return FRotator::ZeroRotator; // 有効なパスがない、または終了している
    }

    FVector CurrentLocation = GetPawn()->GetActorLocation();
    FVector TargetPoint = CurrentPath[CurrentPathIndex];
    

    // ターゲットポイントに十分に近づいたら次のポイントへ
    if (FVector::Distance(CurrentLocation, TargetPoint) < AcceptanceRadius)
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