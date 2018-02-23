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
	DesiredColorGreen = FColor(83, 255, 47, 255);
	float DesiredIntensity = 3000.0f;
	//DesiredColor = DesiredColorBlue;
	PointLight1 = CreateDefaultSubobject<UPointLightComponent>(TEXT("PointLight1"));
	PointLight1->Intensity = DesiredIntensity;
	PointLight1->SetLightColor(DesiredColor); 
	PointLight1->bVisible = true;
	PointLight1->SetupAttachment(RootComponent);

	BoxCollider->OnComponentBeginOverlap.AddDynamic(this, &ASwitchActor::OnOverlapBegin);
	BoxCollider->OnComponentEndOverlap.AddDynamic(this, &ASwitchActor::OnOverlapEnd);
	
	//bGravityStatus = false;
	bReverseGravity = false;
	cooldownTimer = 5.0f;
	bCanTriggerEvent = true;
	bActivatedSwitch = false;
}

void ASwitchActor::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this))
	{
		if(PlayerPawn != nullptr){
			if (OtherActor == PlayerPawn)
			{
				if (winLight) {
					UE_LOG(LogTemp, Warning, TEXT("We win"));
				}
				else {
					ActivateSwitch(bGravityStatus, DesiredColor);

					//GetWorldTimerManager().SetTimer(ResetTimerHandle, &ASwitchActor::ActivateSwitch, 1.0f, true, 2.0f);
					UE_LOG(LogTemp, Warning, TEXT("Overlap Begins"));

					//SwitchGravity(bGravityStatus);				
					//FString compname = OverlappedComp->GetName();
					//FString actorname = OverlappedComp->GetName();
					//bActivatedSwitch = true;
				}				
			}
		}
	}
}

void ASwitchActor::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && (OtherActor != this))
	{
		if (OtherActor == PlayerPawn)
		{
			if (winLight) {
				UE_LOG(LogTemp, Warning, TEXT("We win"));
			}
			else {
				//GetWorldTimerManager().SetTimer(ResetTimerHandle, 1.0f, false, 2.0f);
				ResetSwitch();
				UE_LOG(LogTemp, Warning, TEXT("Actor Ended Overlap"));
			}
		}	
	}
}

void ASwitchActor::ActivateSwitch(bool gravity, FColor color)
{
	UE_LOG(LogTemp, Warning, TEXT("Activate the Switch!"));
	SwitchLightColor(color);
	SwitchGravity(gravity, bReverseGravity);
	
}

void ASwitchActor::SwitchLightColor(FColor color)
{
	UE_LOG(LogTemp, Warning, TEXT("Switch light color!"));
	if (PointLight1->GetLightColor() == DesiredColorRed)
	{
		PointLight1->SetLightColor(DesiredColorBlue);
		bGravityStatus = false;
	}
	else if (PointLight1->GetLightColor() == DesiredColorBlue)
	{
		PointLight1->SetLightColor(DesiredColorRed);
		bGravityStatus = true;
	}
}

void ASwitchActor::SwitchGravity(bool gravity, bool reverse)
{
	if (CustomClassReference != nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("Switch Gravity!"));
		CustomClassReference->GravityLogic(gravity, reverse);
	}
}

void ASwitchActor::ResetSwitch()
{
	UE_LOG(LogTemp, Warning, TEXT("Reset the Switch!"));
	bCanTriggerEvent = true;
	bActivatedSwitch = false;
	cooldownTimer = 1.0f;
}


void ASwitchActor::GetPlayerPawn()
{	
	for (TActorIterator<APlayerPawn> It(GetWorld()); It; ++It)
	{
		UE_LOG(LogTemp, Warning, TEXT("Iterating"));
		PlayerPawn = *It;
		CustomClassReference = *It;
	}
}

// Called when the game starts or when spawned
void ASwitchActor::BeginPlay()
{
	Super::BeginPlay();
	GetPlayerPawn();
	if (DesiredColor == DesiredColorGreen) {
		UE_LOG(LogTemp, Warning, TEXT("Light is green"));
		winLight = true;
		PointLight1->SetLightColor(DesiredColorGreen);
	}
	else if (DesiredColor == DesiredColorRed) {
		winLight = false;
		PointLight1->SetLightColor(DesiredColorRed);
		bGravityStatus = true;
	}
	else if (DesiredColor == DesiredColorBlue) {
		winLight = false;
		PointLight1->SetLightColor(DesiredColorBlue);
		bGravityStatus = false;
	}
}

// Called every frame
void ASwitchActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

