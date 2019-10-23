// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "MyInterface/UnitInterface.h"
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

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void InitCollisionSocket(TArray<FName> socketNames);

	void RequestSockets(TArray<FVector> socketLocations);

private:
	void SwordCollision();

	void CheckCollision();

	UFUNCTION(Server, Reliable)
	void CtoS_SwordCollision();
	void CtoS_SwordCollision_Implementation();

	void ExcuteHitEvent();

	UFUNCTION(Server, Reliable)
	void CtoS_ExcuteHitEvent();
	void CtoS_ExcuteHitEvent_Implementation();
private:
	APawn* OwnerPawn;
	TArray<FHitResult> SaveHits;
	TArray<AActor*> HitObjects;
	TArray<FName> SocketNames;
	TMap<FName, FVector> SocketLocations;

	TMap<FName, TArray<FVector>> SocketLocationsArray;


	bool IsCollisionStart = false;

	UPROPERTY(Replicated)
	bool IsCollisionCheck = false;
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

	DECLARE_DELEGATE_OneParam(FHitEvent, FHitResult);
	FHitEvent OnHitEvent;
public:
	void SetCollisionCheck(bool Result);

	UFUNCTION(Server, Reliable)
	void CtoS_SetCollisionCheck(bool Result);
	void CtoS_SetCollisionCheck_Implementation(bool Result);

	UFUNCTION(NetMulticast, Reliable)
	void StoA_SetCollisionCheck(bool Result);
	void StoA_SetCollisionCheck_Implementation(bool Result);
};
