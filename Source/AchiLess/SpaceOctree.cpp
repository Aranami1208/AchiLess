#include "SpaceOctree.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
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

        UKismetSystemLibrary::PrintString(this, Actor->GetName());
        
        //バウンディングボックスが有効であるとき、判定するオブジェクトとして追加
        if (ObstacleBounds.IsValid)
        {
            AddObstacle(ObstacleBounds);
        }
        
    }

   
     //デバッグ表示 (オプション)
     if (RootNode)
     {
         DrawDebugOctreeNode(RootNode, FColor::Blue);
     }
}

// Called every frame
void ASpaceOctree::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    if (RootNode)
    {
        UKismetSystemLibrary::PrintString(this, "OctreeIsValid");
    }
}

void ASpaceOctree::InitializeOctree(const FVector& CenterLocation, const FVector& InExtent)
{
    // 既存のOctreeをクリア
    if (RootNode)
    {
        RootNode->ClearChildren();
        RootNode->ConditionalBeginDestroy(); // GCにマークする
        RootNode = nullptr;
    }

  
    // ルートノードの境界ボックスを設定
    FBox RootBounds(CenterLocation - InExtent, CenterLocation + InExtent);
    RootNode = NewObject<UOctreeNode>(this);

    RootNode->Initialize(RootBounds);

   
    // Octreeの構築を開始
    //BuildOctreeNode(RootNode);
}

void ASpaceOctree::BuildOctreeNode(UOctreeNode* Node)
{

   
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
        Node->Children[i]->Initialize(ChildBounds);
     
    }
}

void ASpaceOctree::AddObstacle(const FBox& ObstacleBounds)
{
    if (!RootNode)
    {
        UE_LOG(LogTemp, Warning, TEXT("Octree not initialized. Call InitializeOctree first."));
        return;
    }
    AddObstacleToNode(RootNode, ObstacleBounds,0);

    DrawDebugBox(GetWorld(), ObstacleBounds.GetCenter(), ObstacleBounds.GetExtent(), FColor::Yellow, true, -1.0f, 0, 200.0f);
}

void ASpaceOctree::AddObstacleToNode(UOctreeNode* Node, const FBox& ObstacleBounds,int32 Depth)
{
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
        // このノードが既に子を持っていた場合、その子ノードの障害物状態も更新する必要があるか検討。
        // 今回のロジックでは、障害物を含むノードはそれ以上細分化せず、そのノード自体を障害物として扱うため、
        // 子がいても、このノードが障害物とマークされれば、経路探索では通行不可となる。
        // より詳細な制御が必要な場合は、子のクリアや再評価が必要になることも。
        return;
    }

    // 上記の条件に当てはまらず、さらに分割が必要な場合
    // ノードがまだ子を持っていない（リーフノードである）場合、分割を試みる
    if (!Node->HasChildren())
    {
        // MinNodeSize のチェックは BuildOctreeNode (SubdivideNodeOnce) 側でも行われるが、
        // ここでも呼び出す前にチェックしておくと、不要な呼び出しを避けられる。
        // ただし、上の条件で既に MinNodeSize チェックは行われているため、
        // ここに到達した時点で Node->Bounds.GetExtent().GetMax() * 2.0f > MinNodeSize は保証されているはず。
        BuildOctreeNode(Node); // 1段階分割を実行
    }

    // ノードが子を持つようになった (または元々持っていた) 場合、
    // 各子ノードに障害物を再帰的に追加
    // (BuildOctreeNodeを呼んだ結果、MinNodeSizeなどの理由で実際には子が作られないケースも考慮すると、
    //  再度 HasChildren() をチェックするのがより安全)
    if (Node->HasChildren())
    {
        for (UOctreeNode* Child : Node->Children)
        {
            if (Child) // Childがnullptrでないことを確認 (通常は大丈夫なはず)
            {
                AddObstacleToNode(Child, ObstacleBounds,Depth + 1);
            }
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
    if (Node->bContainsObstacle)
    {
        DrawDebugBox(GetWorld(), Node->Bounds.GetCenter(), Node->Bounds.GetExtent(), FColor::Red, true, -1.0f, 0, 700.0f);
    }
    else
    {
        
        DrawDebugBox(GetWorld(), Node->Bounds.GetCenter(), Node->Bounds.GetExtent(), FColor::Blue, true, -1.0f, 0, 500.0f);
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