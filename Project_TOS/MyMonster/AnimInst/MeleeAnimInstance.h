// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MyMonster/Monster.h"
#include "MeleeAnimInstance.generated.h"

/**
 * 
 */

UCLASS()
class PROJECT_TOS_API UMeleeAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
public:
	class AMeleeMonster* MeleeMonster;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		EMonsterState CurrentState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float Speed;
private:
	void UpdateValue();
	void UpdateAnimation();
public:
	UFUNCTION()
		void AnimNotify_StartSimulate(UAnimNotify* Notify);
	UFUNCTION()
		void AnimNotify_AttackEnd(UAnimNotify* Notify);
	UFUNCTION()
		void AnimNotify_HitEnd(UAnimNotify* Notify);
	UFUNCTION()
		void AnimNotify_TraceStart(UAnimNotify* Notify);
	UFUNCTION()
		void AnimNotify_TraceEnd(UAnimNotify* Notify);
};
