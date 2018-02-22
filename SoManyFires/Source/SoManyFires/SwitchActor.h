// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Runtime/Engine/Classes/Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Runtime/Engine/Classes/Components/PointLightComponent.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Classes/Particles/ParticleSystemComponent.h"
#include "SwitchActor.generated.h"

UCLASS()
class SOMANYFIRES_API ASwitchActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASwitchActor();

	// Determine static mesh
	UPROPERTY(EditAnywhere, Category = "Switch Components")
	class UStaticMeshComponent *StaticMesh;

	// Determine box collider
	UPROPERTY(EditAnywhere, Category = "Switch Components")
	class UBoxComponent *BoxCollider;

	UPROPERTY(EditAnywhere, Category = "Switch Components")
	UParticleSystemComponent *OurParticleSystem;

	UPROPERTY(EditAnywhere, Category = "Switch Components")
	UPointLightComponent* PointLight1;
	
	UPROPERTY(EditAnywhere, Category = "Switch Components")
	FColor DesiredColor;

	UPROPERTY(EditAnywhere, Category = "Switch Components")
	bool gravityOn;

	UPROPERTY(EditAnywhere, Category = "Switch Components")
	FColor DesiredColorRed;

	UPROPERTY(EditAnywhere, Category = "Switch Components")
	FColor DesiredColorBlue;

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void SwitchLightColor();

	void SwitchGravity(bool gravity);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
