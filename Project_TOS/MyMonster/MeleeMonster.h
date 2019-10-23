// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Monster.h"
#include "MeleeMonster.generated.h"

UCLASS()
class PROJECT_TOS_API AMeleeMonster : public AMonster
{
	GENERATED_BODY()
public:
	// Sets default values for this character's properties
	AMeleeMonster();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	FVector GetUnitSwordLocation(FName SocketName = "") const;
	virtual FVector GetUnitSwordLocation_Implementation(FName SocketName = "") const override;
public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void OnDeadEvent() override;
	virtual void StartAttack() override;
	virtual void StartHitEvent(float AngleYaw) override;
	virtual void BlackBoardSetting() override;
public:
	void StartSimulate();
public:
	UPROPERTY(EditAnywhere)
		class UAnimMontage* BaseAtkMontage;
};
