// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_LookatRotation.h"
#include "MyMonster/Monster.h"
#include "MyMonster/AIController/MonsterAIController.h"

UBTT_LookatRotation::UBTT_LookatRotation()
{
	bNotifyTick = true;
}
EBTNodeResult::Type UBTT_LookatRotation::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	UObject* SelfActor = OwnerComp.GetBlackboardComponent()->GetValueAsObject(TEXT("SelfActor"));
	AMonster* Monster = Cast<AMonster>(SelfActor);

	if (Monster)
	{
		Monster->StartLookat();
	}

	return EBTNodeResult::InProgress;
}
void UBTT_LookatRotation::TickTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	UObject* SelfActor = OwnerComp.GetBlackboardComponent()->GetValueAsObject(TEXT("SelfActor"));
	AMonster* Monster = Cast<AMonster>(SelfActor);

	if (Monster)
	{
		if (Monster->GetIsLookat())
		{
			OwnerComp.OnTaskFinished(this, EBTNodeResult::Succeeded);
			return;
		}
	}
}
