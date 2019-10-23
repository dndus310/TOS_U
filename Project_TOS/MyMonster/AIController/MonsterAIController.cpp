// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterAIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "MyMonster/MeleeMonster.h"
#include "MyMonster/Monster.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Navigation/CrowdFollowingComponent.h"

//AMonsterAIController::AMonsterAIController()
//{
//	BBComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BBComponent"));
//	BTComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BTComponent"));
//}
AMonsterAIController::AMonsterAIController(const FObjectInitializer & ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UCrowdFollowingComponent>(TEXT("PathFollowingComponent")))
{
	BBComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BBComponent"));
	BTComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BTComponent"));
}

void AMonsterAIController::OnPossess(APawn * InPawn)
{
	Super::OnPossess(InPawn);
	AMonster* Monster = Cast<AMonster>(InPawn);
	
	if (Monster && BTComponent && BBComponent)
	{
		BBComponent->InitializeBlackboard(*(Monster->GetAI()->BlackboardAsset));
		BTComponent->StartTree(*(Monster->GetAI()));
	}
}

void AMonsterAIController::OnUnPossess()
{
	if (BTComponent)
	{
		BTComponent->StopTree();
	}

	Super::OnUnPossess();

}
void AMonsterAIController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult & Result)
{
	Super::OnMoveCompleted(RequestID, Result);

	if (Result.Code.GetValue() == EPathFollowingResult::Success)
	{
		IsMoveComplete = true;
	}
}
void AMonsterAIController::MoveToPoint(FVector Location, float Radius)
{
	MoveToLocation(Location, -1.f, true);
}
uint8 AMonsterAIController::GetCurrentState() const
{
	return BBComponent->GetValueAsEnum(TEXT("CurrentState"));
}
float AMonsterAIController::GetDistance()
{
	return BBComponent->GetValueAsFloat(TEXT("CurrentDistance"));
}
void AMonsterAIController::SetCurrentState(EMonsterState State)
{
	BBComponent->SetValueAsEnum(FName(TEXT("CurrentState")), (uint8)State);
}
void AMonsterAIController::SetTarget(AActor * Target)
{
	BBComponent->SetValueAsObject(FName(TEXT("Target")), Target);
}
void AMonsterAIController::SetLocation(FVector Location)
{
	BBComponent->SetValueAsVector(FName(TEXT("TargetLocation")), Location);
}

void AMonsterAIController::SetDistance(float Dist)
{
	BBComponent->SetValueAsFloat(FName(TEXT("CurrentDistance")), Dist);
}

void AMonsterAIController::SetSlefActor(UObject * Object)
{
	BBComponent->SetValueAsObject(FName(TEXT("SelfActor")), Object);
}
