// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BattlePC.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_TOS_API ABattlePC : public APlayerController
{
	GENERATED_BODY()
public:
	ABattlePC();
public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void SetupInputComponent() override;
	void ShowManualKey();
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class UBattleWidgetBase> BattleWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UBattleWidgetBase* BattleWidgetBase;
};
