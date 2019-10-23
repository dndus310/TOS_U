// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Define.h"
#include "Monster.generated.h"

UENUM()
enum class MonsterState : uint8
{
	Null = 0,
	Attack = 1,
	Chase = 2,
	Strafe = 3,
	BackStep = 4,
	Stun = 5,
	Hit = 6,
	Dead = 7,
};
UCLASS()
class PROJECT_TOS_API AMonster : public ACharacter
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMonster();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FMyDamageType DamageType;
	
	UPROPERTY(EditAnywhere, meta = (Displayname = "ComboSystemComponent"))
		class UComboSystemComponent* ComboSystem;

	UPROPERTY(EditAnywhere, meta = (Displayname = "CollisionComponent"))
		class UCollisionComponent* Collision;
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float Speed = 0.f;

};
