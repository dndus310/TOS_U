// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GM_ProjectTOS.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_TOS_API AGM_ProjectTOS : public AGameModeBase
{
	GENERATED_BODY()
private:
	int MaxLevelCount = 3;
public:
	AGM_ProjectTOS();
	int a;
	void GetComboData();

	UFUNCTION(BlueprintCallable)
	void NextLevel(AActor* actor);
};
