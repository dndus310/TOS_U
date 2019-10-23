// Fill out your copyright notice in the Description page of Project Settings.


#include "RangedAnimInstance.h"
#include "MyMonster/RangedMonster.h"
#include "MyComponent/CollisionComponent.h"

void URangedAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	RangedMonster = Cast<ARangedMonster>(TryGetPawnOwner());

}
void URangedAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!RangedMonster)
	{
		return;
	}

	UpdateAnimation();
}

void URangedAnimInstance::UpdateValue()
{

}

void URangedAnimInstance::UpdateAnimation()
{
	if (!RangedMonster->GetAIC())
	{
		return;
	}
	CurrentState = RangedMonster->GetMonsterState();



	switch (CurrentState)
	{
	case EMonsterState::Default:
		if (!Montage_IsPlaying(RangedMonster->DeadMontage))
		{
			StopAllMontages(0.f);
		}
		break;
	case EMonsterState::Attack:
		/*if (!Montage_IsPlaying(MeleeMonster->BaseAtkMontage))
		{
			MeleeMonster->StartAttack();
		}*/
		break;
	case EMonsterState::Chase:
		break;
	case EMonsterState::Strafe:
		break;
	case EMonsterState::BackStep:
		break;
	case EMonsterState::Stun:
		break;
	case EMonsterState::Hit:
		break;
	case EMonsterState::Dead:
		break;
	default:
		break;
	}
}
void URangedAnimInstance::AnimNotify_AttackEnd(UAnimNotify * Notify)
{
	if (!RangedMonster)
	{
		return;
	}
	RangedMonster->SetAttackEnd(true);
}
void URangedAnimInstance::AnimNotify_TraceStart(UAnimNotify * Notify)
{
	if (!RangedMonster)
	{
		return;
	}
	RangedMonster->Collision->SetCollisionCheck(true);
}

void URangedAnimInstance::AnimNotify_TraceEnd(UAnimNotify * Notify)
{
	if (!RangedMonster)
	{
		return;
	}
	RangedMonster->Collision->SetCollisionCheck(false);
}

void URangedAnimInstance::AnimNotify_FireStart(UAnimNotify * Notify)
{
	if (!RangedMonster)
	{
		return;
	}
	RangedMonster->SpwanFire();
}
