// Fill out your copyright notice in the Description page of Project Settings.


#include "BTS_TargetCheck.h"
#include "MyMonster/Monster.h"
#include "MyMonster/AIController/MonsterAIController.h"

void UBTS_TargetCheck::TickNode(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	Monster = Cast<AMonster>(OwnerComp.GetAIOwner()->GetPawn());

	if (Monster)
	{
		if (Monster->CheckTargetDead())
		{
			Monster->SetMonsterState(EMonsterState::End);
			Monster->GetAIC()->SetTarget(NULL);
		}
		else if (Monster->CheckUnitDead())
		{
			Monster->SetMonsterState(EMonsterState::Dead);
		}
		else
		{
			Monster->GetAIC()->SetDistance(Monster->GetTargetDistXY());
		}
	}
	
}
