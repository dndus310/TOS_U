// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_WaitForAnimFrame.h"
#include "MyMonster/Monster.h"
#include "MyMonster/AIController/MonsterAIController.h"

UBTT_WaitForAnimFrame::UBTT_WaitForAnimFrame()
{
	bNotifyTick = true;
}
EBTNodeResult::Type UBTT_WaitForAnimFrame::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	UObject* SelfActor = OwnerComp.GetBlackboardComponent()->GetValueAsObject(TEXT("SelfActor"));
	Monster = Cast<AMonster>(SelfActor);

	switch (State)
	{
	case EWaitAnimState::BaseAttack:
		if (Monster->GetAttackEnd())
		{
			Monster->SetAttackEnd(false);
		}
		break;
	case EWaitAnimState::BackStep:
		break;
	case EWaitAnimState::Stun:
		break;
	case EWaitAnimState::Hit:
		if (Monster->GetHitEnd())
		{
			Monster->SetHitEnd(false);
		}
		break;
	case EWaitAnimState::Dead:
		break;
	default:
		break;
	}
	return EBTNodeResult::InProgress;
}
void UBTT_WaitForAnimFrame::TickTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	UObject* SelfActor = OwnerComp.GetBlackboardComponent()->GetValueAsObject(TEXT("SelfActor"));
	Monster = Cast<AMonster>(SelfActor);

	switch (State)
	{
	case EWaitAnimState::BaseAttack:
		if (Monster->GetAttackEnd())
		{
			//UE_LOG(LogClass, Error, TEXT("Log message"));

			OwnerComp.OnTaskFinished(this, EBTNodeResult::Succeeded);
		}
		break;
	case EWaitAnimState::BackStep:
		break;
	case EWaitAnimState::Stun:
		break;
	case EWaitAnimState::Hit:
		if (Monster->GetHitEnd())
		{
			OwnerComp.OnTaskFinished(this, EBTNodeResult::Succeeded);
		}
		break;
	case EWaitAnimState::Dead:
		break;
	default:
		break;
	}

}
