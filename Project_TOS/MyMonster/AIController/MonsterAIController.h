// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "MyMonster/MeleeMonster.h"
#include "MonsterAIController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_TOS_API AMonsterAIController : public AAIController
{
	GENERATED_BODY()
public:
	AMonsterAIController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	//AMonsterAIController();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
		class UBehaviorTreeComponent* BTComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
		class UBlackboardComponent* BBComponent;

	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;
	virtual void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) override;
private:
	bool IsMoveComplete = false;
public:
	void MoveToPoint(FVector Location, float Radius = -1.0f);
	void ResetMoveComplete() { IsMoveComplete = false; }
public:
	uint8 GetCurrentState() const;
	bool GetIsCompleteMove() const { return IsMoveComplete; }
	float GetDistance();
public:
	void SetCurrentState(EMonsterState State);
	void SetTarget(AActor* Target);
	void SetLocation(FVector Location);
	void SetDistance(float Dist);
	void SetSlefActor(UObject* Object);
};
