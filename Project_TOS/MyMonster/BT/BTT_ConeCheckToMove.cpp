// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_ConeCheckToMove.h"
#include "MyMonster/AIController/MonsterAIController.h"
#include "MyMonster/Monster.h"
#include "Navigation/PathFollowingComponent.h"

UBTT_ConeCheckToMove::UBTT_ConeCheckToMove() :
	Monster(NULL), TargetLocationXY(FVector::ZeroVector)
{
	bNotifyTick = true;
}
EBTNodeResult::Type UBTT_ConeCheckToMove::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	//UE_LOG(LogTemp, Error, TEXT("Log ExecuteTask"));


	return EBTNodeResult::InProgress;
}
void UBTT_ConeCheckToMove::TickTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	UObject* SelfActor = OwnerComp.GetBlackboardComponent()->GetValueAsObject(TEXT("SelfActor"));
	Monster = Cast<AMonster>(SelfActor);

	if (!Monster)
	{
		//UE_LOG(LogTemp, Error, TEXT("Log TickTask"));

		return;
	}

	if (Monster->GetAIC()->GetCurrentState() == (uint8)EMonsterState::Chase)
	{
		EPathFollowingStatus::Type Status = Monster->GetAIC()->GetMoveStatus();

		if (Monster->CheckRadius(EndPointRadius))
		{
			OwnerComp.OnTaskFinished(this, EBTNodeResult::Succeeded);
		}
		else if (Status != EPathFollowingStatus::Type::Moving)
		{
			TargetLocationXY = Monster->GetTarget()->GetActorLocation();
			TargetLocationXY.Z = Monster->GetActorLocation().Z;
			Monster->GetAIC()->MoveToPoint(TargetLocationXY, EndPointRadius);
		}
		else if (Monster->CheckAngle(InitAngle))
		{
			TargetLocationXY = Monster->GetTarget()->GetActorLocation();
			TargetLocationXY.Z = Monster->GetActorLocation().Z;
			Monster->GetAIC()->MoveToPoint(TargetLocationXY, EndPointRadius);
		}
	}
}