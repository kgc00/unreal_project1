// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerPawn.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InputComponent.h"
#include "Runtime/Engine/Classes/GameFramework/Actor.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Classes/Engine/Engine.h"
#include "Runtime/Engine/Classes/Components/SceneComponent.h"
#include "PlayerPawnMovementComponent.h"


// Sets default values
APlayerPawn::APlayerPawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set this pawn to be controlled by the lowest-numbered player
	AutoPossessPlayer = EAutoReceiveInput::Player0;
	
	// static mesh initialization
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh Component"));
	//StaticMesh->SetupAttachment(RootComponent);

	// Create a dummy root component we can attach things to.
	RootComponent = StaticMesh;
		//CreateDefaultSubobject<UBoxComponent>(TEXT("RootComponent"));
	
	// initialize box
	BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
	BoxCollider->SetupAttachment(RootComponent);
	
	// Create a particle system that we can activate or deactivate
	OurParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("MovementParticles"));
	OurParticleSystem->SetupAttachment(RootComponent);
	OurParticleSystem->bAutoActivate = false;
	OurParticleSystem->SetRelativeLocation(FVector(-20.0f, 0.0f, 20.0f));
	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleAsset(TEXT("/Game/StarterContent/Particles/P_Fire.P_Fire"));
	if (ParticleAsset.Succeeded())
	{
		OurParticleSystem->SetTemplate(ParticleAsset.Object);
	}

	// Use a spring arm to give the camera smooth, natural-feeling motion.
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraAttachmentArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->RelativeRotation = FRotator(-5.f, 0.f, 0.f);
	SpringArm->TargetArmLength = 400.0f;
	SpringArm->bEnableCameraLag = false;
	SpringArm->CameraLagSpeed = 3.0f;

	// Create a camera and attach to our spring arm
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("ActualCamera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);

	// Take control of the default player
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	// Create an instance of our movement component, and tell it to update our root component.
	OurMovementComponent = CreateDefaultSubobject<UPlayerPawnMovementComponent>(TEXT("CustomMovementComponent"));
	OurMovementComponent->UpdatedComponent = RootComponent;

	



	//BoxCollider->OnComponentBeginOverlap.AddDynamic(this, &APlayerPawn::OnOverlapBegin);
}

// Called when the game starts or when spawned
void APlayerPawn::BeginPlay()
{
	Super::BeginPlay();	
	FHitResult *test = new FHitResult;
	FTransform transformTest = GetTransform();
	FVector vectorTest = GetActorLocation();
	FRotator rotatorTest = GetActorRotation();
	FQuat quatTest = GetActorQuat();
	if(StaticMesh != nullptr){
		StaticMesh->SetSimulatePhysics(true);
		StaticMesh->SetEnableGravity(true);
		StaticMesh->SetCollisionObjectType(ECC_Pawn);
		StaticMesh->OnComponentHit.AddDynamic(this, &APlayerPawn::OnHit);
	}
	if (test != nullptr) {
		AActor::SetActorLocationAndRotation(vectorTest, rotatorTest, false, test, ETeleportType::None);
	}
	else { UE_LOG(LogTemp, Warning, TEXT("Nullptr found")); }
}

// Called every frame
void APlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//ApplyGravity();
	FHitResult hr;
	//StaticMesh->SetRelativeLocation(RootComponent->GetComponentLocation(), false, &hr, ETeleportType::None);
	
}

// Called to bind functionality to input
void APlayerPawn::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

	InputComponent->BindAction("Jump", IE_Pressed, this, &APlayerPawn::Jump);
	InputComponent->BindAction("UndefinedAction", IE_Pressed, this, &APlayerPawn::ParticleToggle);
	InputComponent->BindAxis("MoveForward", this, &APlayerPawn::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &APlayerPawn::MoveRight);
	InputComponent->BindAxis("Turn", this, &APlayerPawn::Turn);
}

void APlayerPawn::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit) {
	UE_LOG(LogTemp, Warning, TEXT("We have hit something!"))
}

UPawnMovementComponent* APlayerPawn::GetMovementComponent() const
{
	return OurMovementComponent;
}

void APlayerPawn::MoveForward(float AxisValue)
{	
	/*FVector CurrentLocation = GetActorLocation();
	FVector intendedMovement = (CurrentLocation.X + 10.0f, CurrentLocation, CurrentLocation);
	FHitResult HitResult;
	APlayerPawn::SetActorLocation(intendedMovement, true, &HitResult, ETeleportType::None);*/
	/*OurMovementComponent->AddInputVector(GetActorForwardVector() * AxisValue, true);
	OurMovementComponent->UpdateComponentVelocity();*/

	if (OurMovementComponent && (OurMovementComponent->UpdatedComponent == RootComponent))
	{
		//UE_LOG(LogTemp, Warning, TEXT("Forward"));
		//FHitResult Hit;
		//if (Hit)

		OurMovementComponent->AddInputVector(GetActorForwardVector() * AxisValue * 1000.0f, false);
	}
}

void APlayerPawn::MoveRight(float AxisValue)
{
	if (OurMovementComponent && (OurMovementComponent->UpdatedComponent == RootComponent))
	{
		OurMovementComponent->AddInputVector(GetActorRightVector() * AxisValue, true);
	}
}

void APlayerPawn::Turn(float AxisValue)
{
	FRotator NewRotation = GetActorRotation();
	NewRotation.Yaw += AxisValue;
	SetActorRotation(NewRotation);
}

void APlayerPawn::ParticleToggle()
{
	if (OurParticleSystem && OurParticleSystem->Template)
	{
		OurParticleSystem->ToggleActive();
	}
}

void APlayerPawn::Jump()
{
	
	if (OurMovementComponent && (OurMovementComponent->UpdatedComponent == RootComponent))
	{
		OurMovementComponent->AddInputVector(GetActorUpVector() * jumpStrength);
	}
}

void APlayerPawn::ApplyGravity() {
	if (OurMovementComponent && (OurMovementComponent->UpdatedComponent == RootComponent))
	{
		OurMovementComponent->AddInputVector(GetActorUpVector() * (-gravityModifier));
	}
}