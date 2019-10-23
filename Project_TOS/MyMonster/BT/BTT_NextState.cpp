// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_NextState.h"
#include "MyMonster/Monster.h"
#include "MyMonster/AIController/MonsterAIController.h"

EBTNodeResult::Type UBTT_NextState::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	//AMonster* Monster = Cast<AMonster>(OwnerComp.GetAIOwner()->GetPawn());

	UObject* SelfActor = OwnerComp.GetBlackboardComponent()->GetValueAsObject(TEXT("SelfActor"));
	AMonster* Monster = Cast<AMonster>(SelfActor);

	if (Monster)
	{
		Monster->SetMonsterState(NextState);
	}
	return EBTNodeResult::Succeeded;
}
