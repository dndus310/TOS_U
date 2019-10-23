// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAnimInstance.h"
#include "SwordPlayer.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

void UPlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	CurrentState = AnimState::Idle;
	OriginPlayer = Cast<ASwordPlayer>(TryGetPawnOwner());
}
void UPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!OriginPlayer)
	{
		return;
	}

	UpdateValue();
	UpdateAnimation();
}
void UPlayerAnimInstance::UpdateValue()
{
	Speed = OriginPlayer->GetCharacterMovement()->Velocity.Size();
	Direction = CalculateDirection(OriginPlayer->GetCharacterMovement()->Velocity, OriginPlayer->GetActorRotation());
}
void UPlayerAnimInstance::UpdateAnimation()
{
	//UE_LOG(LogTemp, Log, TEXT("%s"), *OriginPlayer->GetCharacterMovement()->GetMovementName());
	//OriginPlayer->GetCharacterMovement()->StartNewPhysics(0.00001, 1);

	if (OriginPlayer->GetIsDraw() && CurrentLayer == LayerState::BaseLayer)
	{
		CurrentLayer = LayerState::CombatLayer;
	}
	else if (!OriginPlayer->GetIsDraw() && CurrentLayer == LayerState::CombatLayer)
	{
		CurrentLayer = LayerState::BaseLayer;
	}

	switch (CurrentState)
	{
	case AnimState::Idle:
		if (IsMove())
		{
			CurrentState = AnimState::Movement;
			break;
		}
		break;
	case AnimState::Movement:
		if (!IsMove())
		{
			CurrentState = AnimState::Idle;
			break;
		}
		IsSprint = OriginPlayer->GetIsSprint();
		break;
	case AnimState::Attack:
		break;
	case AnimState::Roll:
		break;
	case AnimState::Default:
		OriginPlayer->ResetVariables();
		CurrentState = AnimState::Idle;
		PrevState = AnimState::Default;
		break;
	default:
		break;
	}
	
}
bool UPlayerAnimInstance::IsMove()
{
	if (Speed > 0.f)
	{
		return true;
	}
	else if (Speed <= 0.f)
	{
		return false;
	}
	return false;
}

void UPlayerAnimInstance::AnimNotify_DrawEnd(UAnimNotify* Notify)
{
	if (!OriginPlayer)
	{
		return;
	}
	//if (CurrentState == AnimState::Idle)
	//{
	//	if (CurrentLayer == LayerState::BaseLayer)
	//	{
	//		CurrentLayer = LayerState::CombatLayer;
	//	}
	//	else
	//	{
	//		CurrentLayer = LayerState::BaseLayer;
	//	}
	//}
	IsDrawState = false;
}
void UPlayerAnimInstance::AnimNotify_DrawStart(UAnimNotify * Notify)
{
	if (!OriginPlayer)
	{
		return;
	}
	if (OriginPlayer->GetIsDraw())
	{
		OriginPlayer->WeaponSetParents("Weapon");
	}
	else
	{
		OriginPlayer->WeaponSetParents("Undraw");
	}
	
}
void UPlayerAnimInstance::AnimNotify_AttackEnd(UAnimNotify * Notify)
{
	if (!OriginPlayer)
	{
		return;
	}
	CurrentState = AnimState::Default;

	
}

void UPlayerAnimInstance::AnimNotify_AttackStart(UAnimNotify * Notify)
{
	if (!OriginPlayer)
	{
		return;
	}
}

void UPlayerAnimInstance::AnimNotify_RollEnd(UAnimNotify * Notify)
{
	if (!OriginPlayer)
	{
		return;
	}

	CurrentState = AnimState::Default;
}

void UPlayerAnimInstance::AnimNotify_BackStepEnd(UAnimNotify * Notify)
{
	if (!OriginPlayer)
	{
		return;
	}

	CurrentState = AnimState::Default;
}

void UPlayerAnimInstance::AnimNotify_FlyingStart(UAnimNotify * Notify)
{
	if (!OriginPlayer)
	{
		return;
	}
	CurrentMovementMod = OriginPlayer->GetCharacterMovement()->GetGroundMovementMode();
	
	//OriginPlayer->GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	OriginPlayer->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
	UE_LOG(LogTemp, Log, TEXT("FlyingStart"));
}

void UPlayerAnimInstance::AnimNotify_FlyingEnd(UAnimNotify * Notify)
{
	if (!OriginPlayer)
	{
		return;
	}

	OriginPlayer->GetCharacterMovement()->SetMovementMode(CurrentMovementMod);
	//OriginPlayer->GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
	UE_LOG(LogTemp, Log, TEXT("FlyingEnd"));
}

void UPlayerAnimInstance::AnimNotify_TraceStart(UAnimNotify * Notify)
{
	if (!OriginPlayer)
	{
		return;
	}
	OriginPlayer->SetIsCollisionState(true);
}

void UPlayerAnimInstance::AnimNotify_TraceEnd(UAnimNotify * Notify)
{
	if (!OriginPlayer)
	{
		return;
	}
	OriginPlayer->SetIsCollisionState(false);
}
