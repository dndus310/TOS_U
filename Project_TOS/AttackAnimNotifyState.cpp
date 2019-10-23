// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackAnimNotifyState.h"
#include "Animation/AnimSequenceBase.h"
#include "Components/SkeletalMeshComponent.h"
#include "SwordPlayer.h"


void UAttackAnimNotifyState::NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration)
{
	OriginPlayer = Cast<ASwordPlayer>(MeshComp->GetOwner());

	if (!OriginPlayer)
	{
		return;
	}
	if (OriginPlayer->GetIsSaveAttack())
	{
		OriginPlayer->SetIsSaveAttack(false);
		OriginPlayer->NextComboAttack();
		return;
	}
	OriginPlayer->SetIsComboStart(true);
}
void UAttackAnimNotifyState::NotifyTick(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float FrameDeltaTime)
{
	if (!OriginPlayer)
	{
		return;
	}
	//..exception / hit, dead == combo false 
}

void UAttackAnimNotifyState::NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	if (!OriginPlayer)
	{
		return;
	}

	OriginPlayer->SetIsComboStart(false);

}
