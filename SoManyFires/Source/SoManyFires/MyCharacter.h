// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/Character.h"
#include "MyCharacter.generated.h"

UCLASS()
class SOMANYFIRES_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere)
	USceneComponent* rootC;

	// Determine static mesh
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* CapsuleVisual;

	// Determine particle system
	UParticleSystemComponent* OurParticleSystem;
	class UPlayerPawnMovementComponent* OurMovementComponent;

	// Custom Movement Component
	virtual UPawnMovementComponent* GetMovementComponent() const override;

	//input functions
	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);
	void Turn(float AxisValue);
	void ParticleToggle();
	void Jump();

	// Implement gravity
	UPROPERTY(EditAnywhere)
	float jumpStrength;

	// Implement gravity
	UPROPERTY(EditAnywhere)
	float gravityModifier;

	void ApplyGravity();
};
