#include "MyAIController.h"
#include "Kismet/GameplayStatics.h"
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

}

void AMyAIController::OnPathFindingCompleted(const TArray<FVector>& Path)
{

    //ブラックボードを取得
    UBlackboardComponent* BlackboardComp = GetBlackboardComponent();
    if (!BlackboardComp)
    {
        UE_LOG(LogTemp, Error, TEXT("BlackboardComponent is not valid!"));

        return;
    }

    if (Path.Num() > 0)
    {
        //経路探索が終わったらパスを初期化する
        CurrentPath.Empty();
        CurrentPathIndex = 0;
        CurrentPath = Path;

        int32 Index = 0;

        for (FVector Path : CurrentPath)
        {
            if (Index != CurrentPath.Num() - 1)
                DrawDebugBox(GetWorld(), Path, FVector(1000.f * Index + 1), FColor::Green, true, 0.1f, 0, 700.0f);
            else
                DrawDebugBox(GetWorld(), Path, FVector(1000.f * Index + 1), FColor::Magenta, true, 0.1f, 0, 700.0f);
            Index++;
        }

        UE_LOG(LogTemp, Log, TEXT("Path found with %d points."), CurrentPath.Num());

        //パスを検索出来ているのでtrueを返す
        BlackboardComp->SetValueAsBool(FName("HasPath"), true);
    }
    else
    {
        BlackboardComp->SetValueAsBool(FName("HasPath"), false);
        UE_LOG(LogTemp, Warning, TEXT("Failed to find path."));
    }
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