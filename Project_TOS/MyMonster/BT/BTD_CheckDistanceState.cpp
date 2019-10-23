// Fill out your copyright notice in the Description page of Project Settings.


#include "BTD_CheckDistanceState.h"
#include "MyMonster/Monster.h"
#include "MyMonster/AIController/MonsterAIController.h"

void UBTD_CheckDistanceState::OnNodeActivation(FBehaviorTreeSearchData & SearchData)
{
	Super::OnNodeActivation(SearchData);
}

void UBTD_CheckDistanceState::OnNodeDeactivation(FBehaviorTreeSearchData & SearchData, EBTNodeResult::Type NodeResult)
{
	Super::OnNodeDeactivation(SearchData, NodeResult);
}

void UBTD_CheckDistanceState::OnNodeProcessed(FBehaviorTreeSearchData & SearchData, EBTNodeResult::Type & NodeResult)
{
	Super::OnNodeProcessed(SearchData, NodeResult);
}

bool UBTD_CheckDistanceState::CalculateRawConditionValue(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory) const
{
	return true;
}
