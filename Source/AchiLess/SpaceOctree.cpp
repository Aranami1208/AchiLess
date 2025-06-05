#include "SpaceOctree.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Engine/StaticMeshActor.h"
#include "Subsystems/WorldSubsystem.h"
#include "PathFindingSubsystem.h"
#include "DestructibleActor.h"

// Sets default values
ASpaceOctree::ASpaceOctree()
{
    PrimaryActorTick.bCanEverTick = false; // Octreeは通常Tick不要なのでfalseに設定
    RootNodeIndex = INDEX_NONE;
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
        //除外タグを持っていた場合、処理しない
        if(Actor->ActorHasTag("NoCollisionForPathfinding"))continue;
        //アクターのBoundingBoxを取得
        FBox ObstacleBounds = Actor->GetComponentsBoundingBox();

        UKismetSystemLibrary::PrintString(this, Actor->GetName());
        
        //バウンディングボックスが有効であるとき、判定するオブジェクトとして追加
        if (ObstacleBounds.IsValid)
        {
            AddObstacle(ObstacleBounds);
        }
        
    }
    
    
    if (UWorld* World = GetWorld())
    {
        if (UPathfindingSubsystem* PathSubsystem = World->GetSubsystem<UPathfindingSubsystem>())
        {
            // PathSubsystem に Octree の参照を渡すか、準備完了を通知する関数を呼び出す
            PathSubsystem->SetOctreeReference(this);
            // (この SetOctreeReference は PathfindingSubsystem に自作する必要があります)
            UE_LOG(LogTemp, Log, TEXT("ASpaceOctree notified PathfindingSubsystem about its initialization."));
        }
        else
        {
            UKismetSystemLibrary::PrintString (this, "NoSubsystem");
        }
    }
    else
    {
        UKismetSystemLibrary::PrintString(this, "NoWorld");
    }

    
   
     //デバッグ表示 (オプション)
     if (RootNodeIndex != INDEX_NONE)
     {
         DrawDebugOctreeNode(RootNodeIndex, FColor::Blue);
     }
}

// Called every frame
void ASpaceOctree::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    
}

void ASpaceOctree::InitializeOctree(const FVector& CenterLocation, const FVector& InExtent)
{
    // 既存のノードを全てクリア
    AllNodes.Empty();
    RootNodeIndex = INDEX_NONE;

  
    // ルートノードの境界ボックスを設定
    FBox RootBounds(CenterLocation - InExtent, CenterLocation + InExtent);
   

    // AllNodesに新しいノードを追加し、そのインデックスをルートノードインデックスとする
    RootNodeIndex = AllNodes.Add(FOctreeNode(RootBounds));

}

FOctreeNode ASpaceOctree::GetOctreeNodeAtLocation(const FVector& Location)
{
    if (RootNodeIndex == INDEX_NONE) return FOctreeNode(); // 無効なOctreeNodeを返す

    int32 CurrentNodeIdx = RootNodeIndex;
    while (CurrentNodeIdx != INDEX_NONE)
    {
        FOctreeNode* CurrentNode = GetNode(CurrentNodeIdx);
        if (!CurrentNode) return FOctreeNode(); // 無効なOctreeNodeを返す

        // 現在のノードが障害物を含んでいる場合、この位置はブロックされている
        if (CurrentNode->bContainsObstacle)
        {
            return FOctreeNode(); // 障害物ノードであればパスとして利用できない -> 無効なノードを返す
        }

        // ノードが子ノードを持たない場合（まだ分割されていない、または最小サイズに達した葉ノード）
        // そのノードの範囲内に位置が含まれていれば、そのノードを返す
        if (!CurrentNode->HasChildren())
        {
            if (CurrentNode->Bounds.IsInside(Location))
            {
                return *CurrentNode; // <-- ここでポインタをデリファレンスして値を返す
            }
            return FOctreeNode(); // 葉ノードだが、位置が含まれていない -> 無効なノードを返す
        }

        // 子ノードをチェックし、位置が含まれる子ノードへ移動
        int32 NextNodeIdx = INDEX_NONE;
        for (int32 ChildIdx : CurrentNode->ChildrenIndices)
        {
            const FOctreeNode* ChildNode = GetNode(ChildIdx);
            if (ChildNode && ChildNode->Bounds.IsInside(Location))
            {
                NextNodeIdx = ChildIdx;
                break;
            }
        }

        if (NextNodeIdx != INDEX_NONE)
        {
            CurrentNodeIdx = NextNodeIdx;
        }
        else
        {
            // どのノードにも位置が含まれない（RootNodeの範囲外など）
            return FOctreeNode(); // 無効なOctreeNodeを返す
        }
    }
    return FOctreeNode(); // ここには到達しないはずだが、念のため -> 無効なOctreeNodeを返す
}

void ASpaceOctree::SubdivideNode(int32 NodeIndex)
{

    FOctreeNode* Node = GetNode(NodeIndex);

    UKismetSystemLibrary::PrintString(this, "AddNode");
    /*
    UKismetSystemLibrary::PrintString(this, "BuildExtent"
        + FString::SanitizeFloat(Node->Bounds.GetExtent().X) + ","
        + FString::SanitizeFloat(Node->Bounds.GetExtent().Y) + ","
        + FString::SanitizeFloat(Node->Bounds.GetExtent().Z));
    */
    // ノードのサイズが最小サイズ以下であれば、それ以上分割しない
    if (Node->Bounds.GetExtent().GetMax() * 2.0f <= MinNodeSize)
    {
        UKismetSystemLibrary::PrintString(this, "MinNodeSize : Return");
        return;
    }

    // ノードの中心点を取得
    FVector Center = Node->Bounds.GetCenter();
    // 各子ノードのExtentを計算
    FVector HalfExtent = Node->Bounds.GetExtent() / 2.0f;

    for (int32 i = 0; i < 8; ++i)
    {
        FVector ChildCenter = Center;
        ChildCenter.X += ((i & 1) ? HalfExtent.X : -HalfExtent.X); // X座標
        ChildCenter.Y += ((i & 2) ? HalfExtent.Y : -HalfExtent.Y); // Y座標
        ChildCenter.Z += ((i & 4) ? HalfExtent.Z : -HalfExtent.Z); // Z座標

        FBox ChildBounds(ChildCenter - HalfExtent, ChildCenter + HalfExtent);
       
        Node->ChildrenIndices[i] = AllNodes.Add(FOctreeNode(ChildBounds));
     
    }
}

void ASpaceOctree::AddObstacle(const FBox& ObstacleBounds)
{
    if (RootNodeIndex == INDEX_NONE)
    {
        UE_LOG(LogTemp, Warning, TEXT("Octree not initialized. Call InitializeOctree first."));
        return;
    }
    AddObstacleToNode(RootNodeIndex, ObstacleBounds,0);

    DrawDebugBox(GetWorld(), ObstacleBounds.GetCenter(), ObstacleBounds.GetExtent(), FColor::Yellow, true, -1.0f, 0, 200.0f);
}

void ASpaceOctree::AddObstacleToNode(int32 NodeIndex, const FBox& ObstacleBounds,int32 Depth)
{
    FOctreeNode* Node = GetNode(NodeIndex);
    if (!Node)return;

    if (Depth >= MaxDepth)return;
    //サイズが小さくなったらスキップ
    if (Node->Bounds.GetExtent().GetMax() * 2.0f <= MinNodeSize)return;
    // ノードが障害物と交差していない場合は処理をスキップ
    if (!Node->Bounds.Intersect(ObstacleBounds))
    {
        return;
    }

    // ノードが完全に障害物に含まれている、またはノードが十分に小さい(葉ノードとして扱うべき)場合は障害物としてマーク
    // この条件は、分割を停止し、このノードを障害物としてマークする条件
    if (ObstacleBounds.IsInside(Node->Bounds) )
    {
        Node->bContainsObstacle = true;
        
        return;
    }

    // 上記の条件に当てはまらず、さらに分割が必要な場合
    // ノードがまだ子を持っていない（リーフノードである）場合、分割を試みる
    if (!Node->HasChildren())
    {
        // ただし、上の条件で既に MinNodeSize チェックは行われているため、
        // ここに到達した時点で Node->Bounds.GetExtent().GetMax() * 2.0f > MinNodeSize は保証されているはず。
        SubdivideNode(NodeIndex); // 1段階分割を実行
    }

    // 各子ノードに障害物を再帰的に追加
    for (int32 ChildIndex : Node->ChildrenIndices) // ChildrenIndicesを使用
    {
        if (ChildIndex != INDEX_NONE)
        {
            AddObstacleToNode(ChildIndex, ObstacleBounds,Depth +1);
        }
    }
}

bool ASpaceOctree::IsLocationBlocked(const FVector& Location) const
{
    if (RootNodeIndex == INDEX_NONE)
    {
        return false; // Octreeが初期化されていなければブロックされていないとみなす
    }
    return IsLocationBlockedInNode(RootNodeIndex, Location);
}

bool ASpaceOctree::IsLocationBlockedInNode(int32 NodeIndex, const FVector& Location) const
{
    const FOctreeNode* Node = GetNode(NodeIndex);

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
    for (int32 ChildIdx : Node->ChildrenIndices)
    {
        const FOctreeNode* Child = GetNode(ChildIdx);
        if (Child && Child->Bounds.IsInside(Location))
        {
            return IsLocationBlockedInNode(ChildIdx, Location);
        }
    }

    return false; // どのノードにも含まれない場合はブロックされていない
}

bool ASpaceOctree::IsBoxBlocked(const FBox& Box) const
{
    if (RootNodeIndex == INDEX_NONE)
    {
        return false; // Octreeが初期化されていなければブロックされていないとみなす
    }
    return IsBoxBlockedInNode(RootNodeIndex, Box);
}

FOctreeNode* ASpaceOctree::GetNode(int32 NodeIndex)
{
    if (NodeIndex != INDEX_NONE && AllNodes.IsValidIndex(NodeIndex))
    {
        return &AllNodes[NodeIndex]; // AllNodes は非 const なので、非 const ポインタを返せる
    }
    return nullptr;
}

const FOctreeNode* ASpaceOctree::GetNode(int32 NodeIndex) const
{
    if (NodeIndex != INDEX_NONE && AllNodes.IsValidIndex(NodeIndex))
    {
        return &AllNodes[NodeIndex]; // AllNodes は const と見なされるので、const ポインタを返す
    }
    return nullptr;
}

bool ASpaceOctree::IsBoxBlockedInNode(int32 NodeIndex, const FBox& Box) const
{
    const FOctreeNode* Node = GetNode(NodeIndex);

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
    for (int32 ChildIdx : Node->ChildrenIndices)
    {
        const FOctreeNode* Child = GetNode(ChildIdx);

        if (Child && Child->Bounds.Intersect(Box)) // 子ノードがBoxと交差している場合のみ再帰呼び出し
        {
            if (IsBoxBlockedInNode(ChildIdx, Box))
            {
                return true; // いずれかの子ノードがブロックされていれば全体がブロックされている
            }
        }
    }

    return false; // どのノードもブロックされていない
}

void ASpaceOctree::DrawDebugOctreeNode(int32 NodeIndex, const FColor& Color) const
{

    if (NodeIndex == INDEX_NONE)
    {
        return;
    }
    const FOctreeNode* Node = GetNode(NodeIndex);

    if (!Node)return;

    // デバッグ用のボックスを描画
    if (Node->bContainsObstacle)
    {
        DrawDebugBox(GetWorld(), Node->Bounds.GetCenter(), Node->Bounds.GetExtent(), FColor::Red, true, -1.0f, 0, 700.0f);
    }
    else
    {
        
        DrawDebugBox(GetWorld(), Node->Bounds.GetCenter(), Node->Bounds.GetExtent(), FColor::Blue, true, -1.0f, 0, 500.0f);
    }

    // 子ノードがあれば再帰的に描画
    for (int32 Child : Node->ChildrenIndices)
    {
        if (Child)
        {
            DrawDebugOctreeNode(Child, Color);
        }
    }
}