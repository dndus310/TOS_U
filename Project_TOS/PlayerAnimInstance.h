// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerAnimInstance.generated.h"


UENUM(BlueprintType)
enum class LayerState : uint8
{
	BaseLayer = 0 UMETA(DisplayName = "BaseLayer"),
	CombatLayer = 1 UMETA(DisplayName = "CombatLayer"),
};
UENUM(BlueprintType)
enum class AnimState : uint8
{
	Idle = 0 UMETA(DisplayName = "IDLE"),
	Movement = 1 UMETA(DisplayName = "Movement"),
	Attack = 2 UMETA(DisplayName = "Attack"),
	Roll = 3 UMETA(DisplayName = "Roll"),
	BackStep = 4 UMETA(DisplayName = "BackStep"),
	Default = 5 UMETA(DisplayName = "Default"),
};
/**
 * 
 */
UCLASS()
class PROJECT_TOS_API UPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
private:
	void UpdateValue();
	void UpdateAnimation();

	class ASwordPlayer* OriginPlayer;

	AnimState PrevState = AnimState::Default;
	AnimState CurrentState = AnimState::Default;
	EMovementMode CurrentMovementMod = EMovementMode::MOVE_None;
public:
	UFUNCTION(BlueprintCallable)
	bool IsMove();
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float Speed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float Direction;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool IsSprint;

	bool IsDrawState = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	LayerState CurrentLayer = LayerState::BaseLayer;
public:
	//Getter
	UFUNCTION(BlueprintCallable)
		AnimState GetCurrentState() const { return CurrentState; }
	UFUNCTION(BlueprintCallable)
		LayerState GetLayerState() const { return CurrentLayer; }
	UFUNCTION(BlueprintCallable)
		bool GetIsDrawState() const { return IsDrawState; }

	bool IsCurrentState(AnimState State) const { return CurrentState == State; }
	bool IsLayerState(LayerState State) const { return CurrentLayer == State; }
public:
	//Setter
	void SetLayerState(LayerState State) { CurrentLayer = State; }
	void SetCurrentState(AnimState State) { CurrentState = State; }
	void SetPrevState() { PrevState = CurrentState; }
public:
	//Event
	UFUNCTION()
		void AnimNotify_DrawEnd(UAnimNotify* Notify);
	UFUNCTION()
		void AnimNotify_DrawStart(UAnimNotify* Notify);
	UFUNCTION()
		void AnimNotify_AttackEnd(UAnimNotify* Notify);
	UFUNCTION()
		void AnimNotify_AttackStart(UAnimNotify* Notify);
	UFUNCTION()
		void AnimNotify_RollEnd(UAnimNotify* Notify);
	UFUNCTION()
		void AnimNotify_BackStepEnd(UAnimNotify* Notify);
	UFUNCTION()
		void AnimNotify_FlyingStart(UAnimNotify* Notify);
	UFUNCTION()
		void AnimNotify_FlyingEnd(UAnimNotify* Notify);
	UFUNCTION()
		void AnimNotify_TraceStart(UAnimNotify* Notify);
	UFUNCTION()
		void AnimNotify_TraceEnd(UAnimNotify* Notify);
};
