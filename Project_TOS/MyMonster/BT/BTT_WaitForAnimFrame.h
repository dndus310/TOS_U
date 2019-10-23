// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "MyMonster/Monster.h"
#include "BTT_WaitForAnimFrame.generated.h"


UENUM(BlueprintType)
enum class EWaitAnimState : uint8
{
	BaseAttack = 0,
	BackStep = 1,
	Stun = 2,
	Hit = 3,
	Dead = 4,
};
/**
 * 
 */
UCLASS()
class PROJECT_TOS_API UBTT_WaitForAnimFrame : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTT_WaitForAnimFrame();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EWaitAnimState State;

	AMonster* Monster;
};
