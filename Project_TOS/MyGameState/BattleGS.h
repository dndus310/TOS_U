// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "Define.h"
#include "BattleGS.generated.h"


/**
 * 
 */
UCLASS()
class PROJECT_TOS_API ABattleGS : public AGameState
{
	GENERATED_BODY()
public:
	ABattleGS();
public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
public:
	float CurrentFazeTime;
};
