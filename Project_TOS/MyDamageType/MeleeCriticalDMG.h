// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/DamageType.h"
#include "MeleeCriticalDMG.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_TOS_API UMeleeCriticalDMG : public UDamageType
{
	GENERATED_BODY()
public:
	float DamageAmount = 30.f;
};
