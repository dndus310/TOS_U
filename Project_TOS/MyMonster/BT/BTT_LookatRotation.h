// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_LookatRotation.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_TOS_API UBTT_LookatRotation : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTT_LookatRotation();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
