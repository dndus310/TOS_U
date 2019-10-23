// Fill out your copyright notice in the Description page of Project Settings.


#include "StatusComponent.h"
#include "UnrealNetwork.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"
#include "MyPlayerState/BattlePS.h"
#include "MyPlayer/SwordPlayer.h"
// Sets default values for this component's properties
UStatusComponent::UStatusComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;


	bReplicates = true;
	// ...
}
// Called when the game starts
void UStatusComponent::BeginPlay()
{
	Super::BeginPlay();
	// ...

	//ExcuteDeadEvent();
	
}
void UStatusComponent::SetUnitStatus(float MaxHP, float BaseDamage, float BaseResist, float Neutralize)
{
	this->Status.MaxHP = MaxHP;
	this->Status.CurrentHP = this->Status.MaxHP;
	this->Status.BaseDamage = BaseDamage;
	this->Status.BaseResist = BaseResist;
	this->Status.Neutralize = Neutralize;
}

void UStatusComponent::CheckDead()
{
	if (Status.CurrentHP <= 0.f)
	{
		ExcuteDeadEvent();
		IsDead = true;
	}

	//CtoS_CheckDead();
}
void UStatusComponent::CtoS_CheckDead_Implementation()
{

	StoA_CheckDead();
}
void UStatusComponent::StoA_CheckDead_Implementation()
{
	
}
void UStatusComponent::ExcuteDeadEvent()
{
	OnDeadEvent.ExecuteIfBound();
	IsDead = true;

	CtoS_ExcuteDeadEvent();
}
void UStatusComponent::CtoS_ExcuteDeadEvent_Implementation()
{
	StoA_ExcuteDeadEvent();
}
void UStatusComponent::StoA_ExcuteDeadEvent_Implementation()
{
	
}
void UStatusComponent::ExcuteDamaged(float Damage, EDamagedType eDmgType)
{
	switch (eDmgType)
	{
	case EDamagedType::MeleeLight:
		GetDamaged = Damage - (Damage * Status.BaseResist * 0.01f);
		break;
	case EDamagedType::MeleeHeavy:
		GetDamaged = Damage - (Damage * Status.BaseResist * 0.01f);
		break;
	case EDamagedType::RangedLight:
		GetDamaged = Damage - (Damage * Status.BaseResist * 0.01f);
		break;
	case EDamagedType::RangedHeavy:
		GetDamaged = Damage - (Damage * Status.BaseResist * 0.01f);
		break;
	case EDamagedType::MeleeCritical:
		GetDamaged = Damage - (Damage * Status.BaseResist * 0.01f);
		break;
	case EDamagedType::RangedCritical:
		GetDamaged = Damage - (Damage * Status.BaseResist * 0.01f);
		break;
	default:
		break;
	}

	if (Status.CurrentHP > 0.f)
	{

		if (GetDamaged > 0.f)
		{
			Status.CurrentHP -= GetDamaged;
			//UE_LOG(LogClass, Error, TEXT("Log GetDamaged : %f"), GetDamaged);
			//UE_LOG(LogClass, Error, TEXT("Log CurrentHP : %f"), Status.CurrentHP);


			CurrentHp -= GetDamaged;
			GetDamaged = 0.f;
		}
		else if (GetDamaged <= 0.f)
		{
			Status.CurrentHP -= 1.f;
			//UE_LOG(LogClass, Error, TEXT("Log GetDamaged : %f"), Status.CurrentHP);
			CurrentHp -= 1.f;
			GetDamaged = 0.f;
		}
		ACharacter* Character = Cast<ACharacter>(GetOwner());
		
		if (Character->ActorHasTag("Player"))
		{
			ASwordPlayer* Player = Cast<ASwordPlayer>(Character);
			Player->PlayerPS->SetCurrentHP(Status.CurrentHP);
			//BPS->SetCurrentHP(Status.CurrentHP);
		}
		else
		{
			CheckDead();
		}
		
		
		//CalculateDamaged();
	}
}
float UStatusComponent::GetBindDamage(float DamageAmount)
{
	return Status.BaseDamage + DamageAmount;
}
float UStatusComponent::GetCurrentHp() const
{
	return Status.CurrentHP;
}
// Called every frame
void UStatusComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}
void UStatusComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

