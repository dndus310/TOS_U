// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Define.generated.h"

/**
 * 
 */
UENUM()
enum class ERelational
{
	OR UMETA(DisplayName = "OR"),
	AND UMETA(DisplayName = "AND")
};
UENUM()
enum class EStartMode
{
	SingleMode = 0,
	MultiMode = 1,
};


