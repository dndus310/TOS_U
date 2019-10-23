// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/BlockingVolume.h"
#include "BV_Door.generated.h"



/**
 * 
 */
UCLASS()
class PROJECT_TOS_API ABV_Door : public ABlockingVolume
{
	GENERATED_BODY()

public:
	ABV_Door();
public:
	virtual void BeginPlay() override;

#if WITH_EDITOR
	virtual void LoadedFromAnotherClass(const FName& OldClassName) override;
	virtual void PostEditChangeChainProperty(FPropertyChangedChainEvent& PropertyChangedEvent) override;
#endif //WITH_EDITOR

	UFUNCTION()
	void BeginOverlap(AActor* OverlappedActor, AActor* OtherActor);	
};
