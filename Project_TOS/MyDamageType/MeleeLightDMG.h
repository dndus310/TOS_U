// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/DamageType.h"
#include "MeleeLightDMG.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_TOS_API UMeleeLightDMG : public UDamageType
{
	GENERATED_BODY()
public:
	float DamageAmount = 10.f;
};
