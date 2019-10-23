// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_ResetVariables.h"
#include "MyMonster/Monster.h"

EBTNodeResult::Type UBTT_ResetVariables::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	UObject* SelfActor = OwnerComp.GetBlackboardComponent()->GetValueAsObject(TEXT("SelfActor"));
	AMonster* Monster = Cast<AMonster>(SelfActor);

	if (Monster)
	{
		Monster->ResetVariables();
	}
	return EBTNodeResult::Succeeded;
}
