// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "LobbyPC.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_TOS_API ALobbyPC : public APlayerController
{
	GENERATED_BODY()
public:
	ALobbyPC();

	virtual void BeginPlay() override;
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class ULobbyWidgetBase> LobbyWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class ULobbyWidgetBase* LobbyWidgetBase;

};
