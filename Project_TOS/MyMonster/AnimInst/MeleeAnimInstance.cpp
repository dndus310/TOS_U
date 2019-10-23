// Fill out your copyright notice in the Description page of Project Settings.


#include "MeleeAnimInstance.h"
#include "MyMonster/MeleeMonster.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MyComponent/CollisionComponent.h"

void UMeleeAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	MeleeMonster = Cast<AMeleeMonster>(TryGetPawnOwner());

}
void UMeleeAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!MeleeMonster)
	{
		return;
	}

	UpdateValue();
	UpdateAnimation();
}
void UMeleeAnimInstance::UpdateValue()
{
	if (!MeleeMonster->GetAIC())
	{
		return;
	}
	Speed = MeleeMonster->GetCharacterMovement()->Velocity.Size();

}
void UMeleeAnimInstance::UpdateAnimation()
{
	if (!MeleeMonster->GetAIC())
	{
		return;
	}
	CurrentState = MeleeMonster->GetMonsterState();

	

	switch (CurrentState)
	{
	case EMonsterState::Default:
		if (!Montage_IsPlaying(MeleeMonster->DeadMontage))
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

void UMeleeAnimInstance::AnimNotify_StartSimulate(UAnimNotify * Notify)
{
	if (!MeleeMonster)
	{
		return;
	}
	
	MeleeMonster->StartSimulate();
}

void UMeleeAnimInstance::AnimNotify_AttackEnd(UAnimNotify * Notify)
{
	if (!MeleeMonster)
	{
		return;
	}
	MeleeMonster->SetAttackEnd(true);
}

void UMeleeAnimInstance::AnimNotify_HitEnd(UAnimNotify * Notify)
{
	if (!MeleeMonster)
	{
		return;
	}

	MeleeMonster->SetHitEnd(true);
}

void UMeleeAnimInstance::AnimNotify_TraceStart(UAnimNotify * Notify)
{
	if (!MeleeMonster)
	{
		return;
	}
	MeleeMonster->Collision->SetCollisionCheck(true);
}

void UMeleeAnimInstance::AnimNotify_TraceEnd(UAnimNotify * Notify)
{
	if (!MeleeMonster)
	{
		return;
	}
	MeleeMonster->Collision->SetCollisionCheck(false);
}

