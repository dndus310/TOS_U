// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Monster.h"
#include "RangedMonster.generated.h"

UCLASS()
class PROJECT_TOS_API ARangedMonster : public AMonster
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ARangedMonster();
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		FVector GetUnitSwordLocation(FName SocketName = "") const;
	virtual FVector GetUnitSwordLocation_Implementation(FName SocketName = "") const override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:
	void SpwanFire();
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void BlackBoardSetting() override;
	virtual void OnDeadEvent() override;
	virtual void StartAttack() override;
	virtual void StartHitEvent(float AngleYaw) override;
public:
	UPROPERTY(EditAnywhere)
		class UAnimMontage* BaseAtkMontage;
};
