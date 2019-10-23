// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "StatusComponent.generated.h"


USTRUCT()
struct FUnitStatus
{
	GENERATED_BODY()

	float MaxHP;
	float CurrentHP;
	float BaseDamage;
	float BaseResist;
	float Neutralize;
	float Stamina;
};
UENUM(BlueprintType)
enum class EDamagedType : uint8
{
	MeleeLight = 0,
	MeleeHeavy = 1,
	RangedLight = 2,
	RangedHeavy = 3,
	MeleeCritical = 6,
	RangedCritical = 7 
};
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECT_TOS_API UStatusComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UStatusComponent();

private:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void BeginPlay() override;
private:
	TSubclassOf<UDamageType> DamageTypes;

	float GetDamaged;
	bool IsDead = false;
public:
	FUnitStatus Status;
	float CurrentHp = 100.f;
	DECLARE_DELEGATE(FDeadEvent);
	FDeadEvent OnDeadEvent;
public:
	void CheckDead();

	UFUNCTION(Server, Reliable)
	void CtoS_CheckDead();
	void CtoS_CheckDead_Implementation();

	UFUNCTION(NetMulticast, Reliable)
	void StoA_CheckDead();
	void StoA_CheckDead_Implementation();

	void ExcuteDeadEvent();

	UFUNCTION(Server, Reliable)
	void CtoS_ExcuteDeadEvent();
	void CtoS_ExcuteDeadEvent_Implementation();

	UFUNCTION(NetMulticast, Reliable)
	void StoA_ExcuteDeadEvent();
	void StoA_ExcuteDeadEvent_Implementation();

	void ExcuteDamaged(float Damage, EDamagedType eDmgType);
	float GetBindDamage(float DamageAmount);
public:
	//float GetCurrentHp() const { return Status.CurrentHP; }
	float GetCurrentHp() const;
	float GetMaxHp() const { return Status.MaxHP; }
	float GetAttackDamege() const { return Status.BaseDamage; }
	bool GetDead() const { return IsDead; }
public:
	void SetUnitStatus(float MaxHP, float BaseDamage, float BaseResist, float Neutralize);
};
