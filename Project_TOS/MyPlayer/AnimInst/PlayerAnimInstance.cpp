// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAnimInstance.h"
#include "MyPlayer/SwordPlayer.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MyComponent/CollisionComponent.h"
#include "UnrealNetwork.h"
#include "MyController/BattlePC.h"

void UPlayerAnimInstance::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UPlayerAnimInstance, CurrentLayer);

}

void UPlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	

	
	SetCurrentState(AnimState::Idle);
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

	//if (OriginPlayer->GetIsDraw() && CurrentLayer == LayerState::BaseLayer)
	//{
	//	UE_LOG(LogTemp, Error, TEXT("Log BaseLayer"));

	//	ChangeLayer(LayerState::CombatLayer);
	//	//CurrentLayer = LayerState::CombatLayer;
	//}
	//else if (!OriginPlayer->GetIsDraw() && CurrentLayer == LayerState::CombatLayer)
	//{
	//	UE_LOG(LogTemp, Error, TEXT("Log CombatLayer"));
	//	ChangeLayer(LayerState::BaseLayer);
	//	//CurrentLayer = LayerState::BaseLayer;
	//}

	

	switch (CurrentState)
	{
	case AnimState::Idle:
		if (IsMove())
		{
			SetCurrentState(AnimState::Movement);
			break;
		}
		break;
	case AnimState::Movement:
		if (!IsMove())
		{
			SetCurrentState(AnimState::Idle);
			break;
		}
		IsSprint = OriginPlayer->GetIsSprint();
		break;
	case AnimState::Default:
		//UE_LOG(LogTemp, Error, TEXT("Log AnimState::Default"));
		OriginPlayer->ResetVariables();
		SetCurrentState(AnimState::Idle);
		break;
	case AnimState::Dead:
		OriginPlayer->ResetVariables();
		if (OriginPlayer->DeadMontage != OriginPlayer->GetCurrentMontage())
		{
			StopAllMontages(0.f);
		}
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

void UPlayerAnimInstance::ChangeLayer(LayerState Layer)
{
	if (!OriginPlayer)
	{
		return;
	}

	//if (OriginPlayer->GetController()->IsLocalController())
	//{
	//	CurrentLayer = Layer;
	//}
	CtoS_ChangeLayer(Layer);
}
void UPlayerAnimInstance::CtoS_ChangeLayer_Implementation(LayerState Layer)
{
	CurrentLayer = Layer;
}

void UPlayerAnimInstance::SetCurrentState(AnimState State)
{
	CurrentState = State;
	//CtoS_SetCurrentState(State);
}

void UPlayerAnimInstance::CtoS_SetCurrentState_Implementation(AnimState State)
{

	StoA_SetCurrentState(State);
}

void UPlayerAnimInstance::StoA_SetCurrentState_Implementation(AnimState State)
{
	CurrentState = State;
}

void UPlayerAnimInstance::SetIsDraw(bool State)
{
	IsDrawState = State;
	CtoS_SetIsDraw(State);
}

void UPlayerAnimInstance::CtoS_SetIsDraw_Implementation(bool State)
{
	IsDrawState = State;
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

	SetIsDraw(false);
	//IsDrawState = false;
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
	//CurrentState = AnimState::Default;
	
	SetCurrentState(AnimState::Default);
	//CurrentState = AnimState::Default;
	/*if (OriginPlayer->IsLocallyControlled())
	{
		OriginPlayer->ResetVariables();
	}*/
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
	SetCurrentState(AnimState::Default);
	//CurrentState = AnimState::Default;
	//OriginPlayer->ResetVariables();
}
void UPlayerAnimInstance::AnimNotify_BackStepEnd(UAnimNotify * Notify)
{
	if (!OriginPlayer)
	{
		return;
	}
	SetCurrentState(AnimState::Default);
	//CurrentState = AnimState::Default;
	//OriginPlayer->ResetVariables();
}
void UPlayerAnimInstance::AnimNotify_FlyingStart(UAnimNotify * Notify)
{
	if (!OriginPlayer)
	{
		return;
	}
	CurrentMovementMod = OriginPlayer->GetCharacterMovement()->GetGroundMovementMode();
	//OriginPlayer->GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	
	OriginPlayer->SetCharacterMovementMode(EMovementMode::MOVE_Flying);
	OriginPlayer->SetCollisionPresset(TEXT("GhostPresset"));

	//OriginPlayer->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);

	//UE_LOG(LogTemp, Log, TEXT("FlyingStart"));
}
void UPlayerAnimInstance::AnimNotify_FlyingEnd(UAnimNotify * Notify)
{
	if (!OriginPlayer)
	{
		return;
	}
	//OriginPlayer->GetCharacterMovement()->SetMovementMode(CurrentMovementMod);
	OriginPlayer->SetCharacterMovementMode(CurrentMovementMod);
	OriginPlayer->SetCollisionPresset(TEXT("PlayerPresset"));
	//OriginPlayer->GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
	//UE_LOG(LogTemp, Log, TEXT("FlyingEnd"));
}
void UPlayerAnimInstance::AnimNotify_TraceStart(UAnimNotify * Notify)
{
	if (!OriginPlayer)
	{
		return;
	}
	if (OriginPlayer->IsLocallyControlled())
	{
		OriginPlayer->Collision->SetCollisionCheck(true);
	}
}
void UPlayerAnimInstance::AnimNotify_TraceEnd(UAnimNotify * Notify)
{
	if (!OriginPlayer)
	{
		return;
	}
	if (OriginPlayer->IsLocallyControlled())
	{
		OriginPlayer->Collision->SetCollisionCheck(false);
	}
}

void UPlayerAnimInstance::AnimNotify_MeleeLightDMG(UAnimNotify * Notify)
{
	if (!OriginPlayer)
	{
		return;
	}
	OriginPlayer->SetDamagedType(EDamagedType::MeleeLight);
}

void UPlayerAnimInstance::AnimNotify_MeleeHeavyDMG(UAnimNotify * Notify)
{
	if (!OriginPlayer)
	{
		return;
	}
	OriginPlayer->SetDamagedType(EDamagedType::MeleeHeavy);
}
