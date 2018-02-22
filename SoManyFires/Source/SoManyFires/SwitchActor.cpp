// Fill out your copyright notice in the Description page of Project Settings.

#include "SwitchActor.h"


// Sets default values
ASwitchActor::ASwitchActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
	BoxCollider->bGenerateOverlapEvents = true;
	BoxCollider->InitBoxExtent(FVector(50, 50, 50));
	RootComponent = BoxCollider;
	
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh Component"));
	StaticMesh->SetupAttachment(RootComponent);

	OurParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("MovementParticles"));
	OurParticleSystem->SetupAttachment(RootComponent);
	OurParticleSystem->bAutoActivate = false;
	OurParticleSystem->SetRelativeLocation(FVector(-20.0f, 0.0f, 20.0f));
	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleAsset(TEXT("/Game/StarterContent/Particles/P_Sparks.P_Sparks"));
	if (ParticleAsset.Succeeded())
	{
		OurParticleSystem->SetTemplate(ParticleAsset.Object);
	}

	DesiredColorRed = FColor(255, 47, 83, 255);
	DesiredColorBlue = FColor(47, 83, 255, 255);
	float DesiredIntensity = 3000.0f;
	DesiredColor = DesiredColorRed;
	PointLight1 = CreateDefaultSubobject<UPointLightComponent>(TEXT("PointLight1"));
	PointLight1->Intensity = DesiredIntensity;
	PointLight1->SetLightColor(DesiredColor); 
	PointLight1->bVisible = true;
	PointLight1->SetupAttachment(RootComponent);

	BoxCollider->OnComponentBeginOverlap.AddDynamic(this, &ASwitchActor::OnOverlapBegin);
	BoxCollider->OnComponentEndOverlap.AddDynamic(this, &ASwitchActor::OnOverlapEnd);
	
}

void ASwitchActor::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this))
	{
		UE_LOG(LogTemp, Warning, TEXT("Actor Started Overlap"));
	}
}

void ASwitchActor::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && (OtherActor != this))
	{
		UE_LOG(LogTemp, Warning, TEXT("Actor Ended Overlap"));
	}
}

void ASwitchActor::SwitchLightColor()
{
	if (PointLight1->GetLightColor == DesiredColorRed)
	{
		PointLight1->SetLightColor(DesiredColorBlue);
		gravityOn = false;
	}
	else if (PointLight1->GetLightColor == DesiredColorBlue)
	{
		PointLight1->SetLightColor(DesiredColorRed);
		gravityOn = true;
	}
}

void ASwitchActor::SwitchGravity(bool gravity)
{
	if (gravity == true) {
		// turn on gravity for pawn
	}
	else {
		// turn off gravity for pawn
	}
}

// Called when the game starts or when spawned
void ASwitchActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASwitchActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

