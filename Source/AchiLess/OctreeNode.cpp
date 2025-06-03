// Fill out your copyright notice in the Description page of Project Settings.


#include "OctreeNode.h"

void UOctreeNode::Initialize(const FBox& InBounds)
{
    Bounds = InBounds;
    bContainsObstacle = false;
}

bool UOctreeNode::HasChildren() const
{
    return Children.Num() > 0;
}

void UOctreeNode::ClearChildren()
{
    for (UOctreeNode* Child : Children)
    {
        if (Child)
        {
            Child->ClearChildren(); // 再帰的にクリア
            // UObjectはガベージコレクションによって管理されるため、deleteは不要
            // Child->MarkAsGarbage(); // 明示的にGCにマークすることも可能だが、親が破棄されれば自動的に行われる
        }
    }
    Children.Empty();
}
