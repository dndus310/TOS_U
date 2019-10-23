// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LobbyGM.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_TOS_API ALobbyGM : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;
public:
	void NextLevel();
};
