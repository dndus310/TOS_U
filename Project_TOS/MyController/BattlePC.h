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

	void ShowPlayerInfo();
	void ShowManualKey();
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class UBattleWidgetBase> BattleWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class UPlayerInfoWidgetBase> PlayerInfoWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UBattleWidgetBase* BattleWidgetBase;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UPlayerInfoWidgetBase* PlayerInfoWidgetBase;
};
