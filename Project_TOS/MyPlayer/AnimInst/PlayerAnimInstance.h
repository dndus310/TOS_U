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
	Dead = 6 UMETA(DisplayName = "Dead"),
};
/**
 * 
 */
UCLASS()
class PROJECT_TOS_API UPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:

public:
	virtual void GetLifetimeReplicatedProps(TArray< class FLifetimeProperty > & OutLifetimeProps) const;
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		AnimState CurrentState = AnimState::Default;
private:
	void UpdateValue();
	void UpdateAnimation();

	class ASwordPlayer* OriginPlayer;


	EMovementMode CurrentMovementMod = EMovementMode::MOVE_None;
public:
	UFUNCTION(BlueprintCallable)
	bool IsMove();
	bool IsCurrentState(AnimState State) const { return CurrentState == State; }
	bool IsLayerState(LayerState State) const { return CurrentLayer == State; }

	void ChangeLayer(LayerState Layer);

	UFUNCTION(Server, Reliable)
	void CtoS_ChangeLayer(LayerState Layer);
	void CtoS_ChangeLayer_Implementation(LayerState Layer);

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float Speed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float Direction;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool IsSprint;

	bool IsDrawState = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated)
	LayerState CurrentLayer = LayerState::BaseLayer;
public:
	//Getter
	UFUNCTION(BlueprintCallable)
		AnimState GetCurrentState() const { return CurrentState; }
	UFUNCTION(BlueprintCallable)
		LayerState GetLayerState() const { return CurrentLayer; }
	UFUNCTION(BlueprintCallable)
		bool GetIsDrawState() const { return IsDrawState; }

public:
	//Setter
	void SetLayerState(LayerState State) { CurrentLayer = State; }


	void SetCurrentState(AnimState State);
	
	UFUNCTION(Server, Reliable)
	void CtoS_SetCurrentState(AnimState State);
	void CtoS_SetCurrentState_Implementation(AnimState State);

	UFUNCTION(NetMulticast, Reliable)
	void StoA_SetCurrentState(AnimState State);
	void StoA_SetCurrentState_Implementation(AnimState State);
	
		
	void SetIsDraw(bool State);

	UFUNCTION(Server, Reliable)
	void CtoS_SetIsDraw(bool State);
	void CtoS_SetIsDraw_Implementation(bool State);

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
	UFUNCTION()
		void AnimNotify_MeleeLightDMG(UAnimNotify* Notify);
	UFUNCTION()
		void AnimNotify_MeleeHeavyDMG(UAnimNotify* Notify);
};
