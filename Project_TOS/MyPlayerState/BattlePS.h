// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "BattlePS.generated.h"

/**
 * 
 */
class ASwordPlayer;

UCLASS()
class PROJECT_TOS_API ABattlePS : public APlayerState
{
	GENERATED_BODY()
public:
	ABattlePS();
public:
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
public:
	float CurrentHP;

	ASwordPlayer* Player;
public:
	void SetCurrentHP(float Value);
	
	void SetPlayer(ASwordPlayer* Value);

	UFUNCTION(Server, Reliable)
	void CtoS_SetCurrentHP(float Value);
	void CtoS_SetCurrentHP_Implementation(float Value);

	UFUNCTION(NetMulticast, Reliable)
	void StoA_SetCurrentHP(float Value);
	void StoA_SetCurrentHP_Implementation(float Value);

};
