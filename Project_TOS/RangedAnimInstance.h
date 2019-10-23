
#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
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
};
