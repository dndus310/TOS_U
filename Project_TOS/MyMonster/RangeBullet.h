// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RangeBullet.generated.h"

UCLASS()
class PROJECT_TOS_API ARangeBullet : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARangeBullet();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
public:

	UPROPERTY(EditAnywhere)
		class USphereComponent* SphereCollision;

	UPROPERTY(EditAnywhere)
		class UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(EditAnywhere)
		class UParticleSystemComponent* Particle;

	UFUNCTION()
		void OnBeginOverlap(AActor* OverlappedActor, AActor* OtherActor);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UParticleSystem* Explosion;
};
