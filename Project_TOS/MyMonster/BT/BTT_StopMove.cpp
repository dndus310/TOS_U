// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_StopMove.h"
#include "MyMonster/AIController/MonsterAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MyMonster/Monster.h"

EBTNodeResult::Type UBTT_StopMove::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	UObject* SelfActor = OwnerComp.GetBlackboardComponent()->GetValueAsObject(TEXT("SelfActor"));
	AMonster* Monster = Cast<AMonster>(SelfActor);

	AMonsterAIController* AIC = Monster->GetAIC();

	if (AIC && Monster)
	{
		Monster->GetCharacterMovement()->Velocity = FVector::ZeroVector;
		AIC->StopMovement();
	}
	return EBTNodeResult::Succeeded;
}
