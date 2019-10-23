// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTS_TargetCheck.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_TOS_API UBTS_TargetCheck : public UBTService
{
	GENERATED_BODY()
public:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	class AMonster* Monster = NULL;
};
