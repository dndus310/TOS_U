
#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MyMonster/Monster.h"
#include "RangedAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_TOS_API URangedAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
public:
	class ARangedMonster* RangedMonster;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		EMonsterState CurrentState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float Speed;
private:
	void UpdateValue();
	void UpdateAnimation();
public:
	UFUNCTION()
		void AnimNotify_AttackEnd(UAnimNotify* Notify);
	UFUNCTION()
		void AnimNotify_TraceStart(UAnimNotify* Notify);
	UFUNCTION()
		void AnimNotify_TraceEnd(UAnimNotify* Notify);
	UFUNCTION()
		void AnimNotify_FireStart(UAnimNotify* Notify);
};
