// Copyright Jared Hutton 2020

#include "Grabber.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	FindPhysicsHandle();	
	SetUpInputComponent();
}

void UGrabber::SetUpInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if(InputComponent)
	{
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
}

	// check for physics handle 
void UGrabber::FindPhysicsHandle()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if(PhysicsHandle)
	{
		// the physicshandle is found
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s does not have a Physics Handle."), *GetOwner()->GetName());
	}
}

void UGrabber::Grab()
{
	UE_LOG(LogTemp, Warning, TEXT("Grabber Pressed."));

	FVector PlayerLocation;
	FRotator PlayerRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerLocation, OUT PlayerRotation);

	FVector LineTraceEnd = PlayerLocation + PlayerRotation.Vector() * Reach;

	FHitResult HitResult = GetFirstPhysicsBodyInReach();
	UPrimitiveComponent* ComponentToGrab = HitResult.GetComponent();

	// if we hit something, then attach the physics handle
	if(HitResult.GetActor())
	{
		PhysicsHandle->GrabComponentAtLocation
		(
			ComponentToGrab,
			NAME_None,
			LineTraceEnd
		);
	}
}

void UGrabber::Release()
{
	UE_LOG(LogTemp, Warning, TEXT("Grabber Released."));

	// remove the physics handle
	PhysicsHandle->ReleaseComponent();
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FVector PlayerLocation;
	FRotator PlayerRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerLocation, OUT PlayerRotation);

	FVector LineTraceEnd = PlayerLocation + PlayerRotation.Vector() * Reach;

	// if physics handle is attached
	if(PhysicsHandle->GrabbedComponent)
	{
		// move object we are holding
		PhysicsHandle->SetTargetLocation(LineTraceEnd);
	}
	

}

FHitResult UGrabber::GetFirstPhysicsBodyInReach() const
{
	// Get player's viewpoint
	FVector PlayerLocation;
	FRotator PlayerRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerLocation, OUT PlayerRotation);

	FVector LineTraceEnd = PlayerLocation + PlayerRotation.Vector() * Reach;

	FHitResult Hit;
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());
	// Ray-cast out to a certain distance (Reach)
	GetWorld()->LineTraceSingleByObjectType
	(
		OUT Hit,
		PlayerLocation,
		LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParams
	);
	
	// See what it hits
	AActor* ActorHit = Hit.GetActor();

	// Log out to test
	if(ActorHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("Line trace hit %s"), *ActorHit->GetName());
	}

	return Hit;
}