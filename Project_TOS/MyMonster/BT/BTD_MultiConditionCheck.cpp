// Fill out your copyright notice in the Description page of Project Settings.


#include "BTD_MultiConditionCheck.h"
#include "BehaviorTree/Decorators/BTDecorator_Blackboard.h"
#include "MyMonster/AIController/MonsterAIController.h"
#include "MyMonster/Monster.h"

void UBTD_MultiConditionCheck::OnNodeActivation(FBehaviorTreeSearchData& SearchData)
{
	Super::OnNodeActivation(SearchData);
}
void UBTD_MultiConditionCheck::OnNodeDeactivation(FBehaviorTreeSearchData& SearchData, EBTNodeResult::Type NodeResult)
{
	Super::OnNodeDeactivation(SearchData, NodeResult);
}
void UBTD_MultiConditionCheck::OnNodeProcessed(FBehaviorTreeSearchData& SearchData, EBTNodeResult::Type& NodeResult)
{
	Super::OnNodeProcessed(SearchData, NodeResult);
}
bool UBTD_MultiConditionCheck::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	if (ConditionArray.Num() < 1)
	{
		return false;
	}
	bool isResult = false;
	AMonster* Monster = Cast<AMonster>(OwnerComp.GetAIOwner()->GetPawn());

	for (int i = 0; i < ConditionArray.Num(); ++i)
	{
		if (Monster->IsMonsterState(ConditionArray[i]))
		{
			if (ECurrentRelational == ERelational::OR)
			{
				return true;
			}
			else
			{
				isResult = true;
			}
		}
		else
		{
			isResult = false;
		}
	}
	return isResult;
}