#include "SpaceOctree.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/StaticMeshActor.h"
#include "DestructibleActor.h"

// Sets default values
ASpaceOctree::ASpaceOctree()
{
    PrimaryActorTick.bCanEverTick = false; // Octreeは通常Tick不要なのでfalseに設定
    RootNode = nullptr;
}

// Called when the game starts or when spawned
void ASpaceOctree::BeginPlay()
{
    Super::BeginPlay();

    // デフォルトの中心とExtentでOctreeを初期化
    InitializeOctree(GetActorLocation(), Extent);

    // シーン内のすべてのAMeteor Actorを検索し、Octreeに障害物として追加
    TArray<AActor*> FoundObstacleActors;

    //破壊可能オブジェクト
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADestructibleActor::StaticClass(), FoundObstacleActors);
    //レベルに直接配置したStaticMesh
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AStaticMeshActor::StaticClass(), FoundObstacleActors);

    for (AActor* Actor : FoundObstacleActors)
    {
        //アクターのBoundingBoxを取得
        FBox ObstacleBounds = Actor->GetComponentsBoundingBox();
        
        //バウンディングボックスが有効であるとき、判定するオブジェクトとして追加
        if (ObstacleBounds.IsValid)
        {
            AddObstacle(ObstacleBounds);
        }
    }

   
}

// Called every frame
void ASpaceOctree::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    // デバッグ表示 (オプション)
    if (RootNode)
    {
        DrawDebugOctreeNode(RootNode, FColor::Blue);
    }
}

void ASpaceOctree::InitializeOctree(const FVector& CenterLocation, const FVector& InExtent)
{
    // 既存のOctreeをクリア
    if (RootNode)
    {
        RootNode->ClearChildren();
        delete RootNode;
        RootNode = nullptr;
    }

    // ルートノードの境界ボックスを設定
    FBox RootBounds(CenterLocation - InExtent, CenterLocation + InExtent);
    RootNode = NewObject<UOctreeNode>(this);

    // Octreeの構築を開始
    BuildOctreeNode(RootNode);
}

void ASpaceOctree::BuildOctreeNode(UOctreeNode* Node)
{
    // ノードのサイズが最小サイズ以下であれば、それ以上分割しない
    if (Node->Bounds.GetExtent().GetMax() * 2.0f <= MinNodeSize)
    {
        return;
    }

    // ノードの中心点を取得
    FVector Center = Node->Bounds.GetCenter();
    // 各子ノードのExtentを計算
    FVector HalfExtent = Node->Bounds.GetExtent() / 2.0f;

    // 8つの子ノードを生成
    Node->Children.SetNum(8);
    for (int32 i = 0; i < 8; ++i)
    {
        FVector ChildCenter = Center;
        ChildCenter.X += ((i & 1) ? HalfExtent.X : -HalfExtent.X); // X座標
        ChildCenter.Y += ((i & 2) ? HalfExtent.Y : -HalfExtent.Y); // Y座標
        ChildCenter.Z += ((i & 4) ? HalfExtent.Z : -HalfExtent.Z); // Z座標

        FBox ChildBounds(ChildCenter - HalfExtent, ChildCenter + HalfExtent);
        Node->Children[i] = NewObject<UOctreeNode>(Node);

        // 再帰的に子ノードを構築
        BuildOctreeNode(Node->Children[i]);
    }
}

void ASpaceOctree::AddObstacle(const FBox& ObstacleBounds)
{
    if (!RootNode)
    {
        UE_LOG(LogTemp, Warning, TEXT("Octree not initialized. Call InitializeOctree first."));
        return;
    }
    AddObstacleToNode(RootNode, ObstacleBounds);
}

void ASpaceOctree::AddObstacleToNode(UOctreeNode* Node, const FBox& ObstacleBounds)
{
    // ノードが障害物と交差していない場合は処理をスキップ
    if (!Node->Bounds.Intersect(ObstacleBounds))
    {
        return;
    }

    // ノードが完全に障害物に含まれている、またはノードが十分に小さい場合は障害物としてマーク
    if (ObstacleBounds.IsInside(Node->Bounds) || Node->Bounds.GetExtent().GetMax() * 2.0f <= MinNodeSize)
    {
        Node->bContainsObstacle = true;
        return;
    }

    // 子ノードが存在しない場合は生成（初期化時に全て生成済みだが、念のため）
    if (!Node->HasChildren())
    {
        BuildOctreeNode(Node);
    }

    // 各子ノードに障害物を再帰的に追加
    for (UOctreeNode* Child : Node->Children)
    {
        if (Child)
        {
            AddObstacleToNode(Child, ObstacleBounds);
        }
    }
}

bool ASpaceOctree::IsLocationBlocked(const FVector& Location) const
{
    if (!RootNode)
    {
        return false; // Octreeが初期化されていなければブロックされていないとみなす
    }
    return IsLocationBlockedInNode(RootNode, Location);
}

bool ASpaceOctree::IsLocationBlockedInNode(UOctreeNode* Node, const FVector& Location) const
{
    // ノードが障害物を含んでいる場合、その場所はブロックされている
    if (Node->bContainsObstacle)
    {
        return true;
    }

    // ノードが子ノードを持たない場合（葉ノード）、ブロックされていない
    if (!Node->HasChildren())
    {
        return false;
    }

    // どのFVectorがLocationを含んでいるか検索
    for (UOctreeNode* Child : Node->Children)
    {
        if (Child && Child->Bounds.IsInside(Location))
        {
            return IsLocationBlockedInNode(Child, Location);
        }
    }

    return false; // どのノードにも含まれない場合はブロックされていない
}

bool ASpaceOctree::IsBoxBlocked(const FBox& Box) const
{
    if (!RootNode)
    {
        return false; // Octreeが初期化されていなければブロックされていないとみなす
    }
    return IsBoxBlockedInNode(RootNode, Box);
}

bool ASpaceOctree::IsBoxBlockedInNode(UOctreeNode* Node, const FBox& Box) const
{
    // ノードが障害物を含んでいる場合、そのノードとBoxが交差していればブロックされている
    if (Node->bContainsObstacle && Node->Bounds.Intersect(Box))
    {
        return true;
    }

    // ノードが子ノードを持たない場合（葉ノード）、ブロックされていない
    if (!Node->HasChildren())
    {
        return false;
    }

    // 子ノードを再帰的にチェック
    for (UOctreeNode* Child : Node->Children)
    {
        if (Child && Child->Bounds.Intersect(Box)) // 子ノードがBoxと交差している場合のみ再帰呼び出し
        {
            if (IsBoxBlockedInNode(Child, Box))
            {
                return true; // いずれかの子ノードがブロックされていれば全体がブロックされている
            }
        }
    }

    return false; // どのノードもブロックされていない
}

void ASpaceOctree::DrawDebugOctreeNode(const UOctreeNode* Node, const FColor& Color) const
{
    // デバッグ用のボックスを描画
    DrawDebugBox(GetWorld(), Node->Bounds.GetCenter(), Node->Bounds.GetExtent(), Color, false, -1.0f, 0, 10.0f);

    if (Node->bContainsObstacle)
    {
        DrawDebugBox(GetWorld(), Node->Bounds.GetCenter(), Node->Bounds.GetExtent(), FColor::Red, false, -1.0f, 0, 15.0f);
    }

    // 子ノードがあれば再帰的に描画
    for (UOctreeNode* Child : Node->Children)
    {
        if (Child)
        {
            DrawDebugOctreeNode(Child, Color);
        }
    }
}