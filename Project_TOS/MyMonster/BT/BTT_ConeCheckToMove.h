// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "MyMonster/Monster.h"
#include "BTT_ConeCheckToMove.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_TOS_API UBTT_ConeCheckToMove : public UBTTaskNode
{
	GENERATED_BODY()

private:
	EBTNodeResult::Type MoveTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);
public:
	UBTT_ConeCheckToMove();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float InitAngle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float EndPointRadius = 1.f;

	EBTNodeResult::Type NodeResult = EBTNodeResult::InProgress;
	bool IsMove = false;

	AMonster* Monster;
	FVector TargetLocationXY;
};
