// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Components/StaticMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "Particles/ParticleSystemComponent.h"
#include "Runtime/Engine/Classes/GameFramework/SpringArmComponent.h"
#include "Runtime/Engine/Classes/Components/BoxComponent.h"
#include "PlayerPawn.generated.h"

UCLASS()
class SOMANYFIRES_API APlayerPawn : public APawn
{
	GENERATED_BODY()
public:
	// Sets default values for this pawn's properties
	APlayerPawn(); 

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

	// Determine static mesh
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USpringArmComponent* SpringArm;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCameraComponent* Camera;

	// Determine box collider
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBoxComponent* BoxCollider;

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
	bool isJumping;

	// Implement gravity
	UPROPERTY(EditAnywhere)
	float jumpStrength;

	// Implement gravity
	UPROPERTY(EditAnywhere)
	float gravityModifier;

	void GravityLogic(bool GravityStatus, bool ReverseGravity);	

	// Timer logic
	UPROPERTY(EditAnywhere)
	int32 jumpTimer;
	FTimerHandle JumpTimerHandle;
	UFUNCTION(BlueprintNativeEvent)
	void JumpTimerFinished();
	virtual void JumpTimerFinished_Implementation();
	void AdvanceJumpTimer();	
	//void SetPhysicsLinearVelocity(FVector NewVel, bool bAddToCurrent, FName BoneName);
	FVector gravityDirection = FVector(0.f,0.f,0.f);
	FVector gravityDirectionUp = FVector(0.f, 0.f, -9.8f);
	FVector gravityDirectionDown = FVector(0.f, 0.f, 9.8f);
	bool gravityUp;
	bool gravityOn;
	FVector ActorStartLocation;
	FRotator ActorStartRotation;
	void PlayerDied();
	bool hasRespawned;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
