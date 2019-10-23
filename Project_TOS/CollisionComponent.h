// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "CollisionComponent.generated.h"

UENUM()
enum class EDrawType
{
	None,
	ForOneFrame,
	ForDuration,
	Persistent
};
UCLASS(Blueprintable,ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECT_TOS_API UCollisionComponent : public UActorComponent
{
	GENERATED_BODY()
public:	
	// Sets default values for this component's properties
	UCollisionComponent();
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	bool LineTraceObjects(FVector Start, FVector End, TArray<AActor*> &OutObejcts, TArray<FHitResult> &HitObjects);
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray< TEnumAsByte<EObjectTypeQuery>> ObjectTypes = 
	{ 
		UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic),
		UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic),
		UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn),
		UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_PhysicsBody),
	};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bTraceComlex = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DrawTime = 5.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bIgnoreSelf = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EDrawType DrawDebugTrace = EDrawType::ForDuration;
};
