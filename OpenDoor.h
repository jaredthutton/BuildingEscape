// Copyright Jared Hutton 2020

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"
#include "OpenDoor.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void OpenDoor(float DeltaTime);
	void CloseDoor(float DeltaTime);

private:

	UPROPERTY(EditAnywhere)
	float OpenAngle = 90.f;
	float CurrentYaw;
	float InitialYaw;


	float DoorLastOpened = 0.f;

	UPROPERTY(EditAnywhere)
	float DoorCloseDelay = 0.25f;

	UPROPERTY(EditAnywhere)
	float DoorOpenSpeed = 3.f;

	UPROPERTY(EditAnywhere)
	float DoorCloseSpeed = 5.f;

	UPROPERTY(EditAnywhere)
	ATriggerVolume* PressurePlate;	
	UPROPERTY(EditAnywhere)
	AActor* ActorThatOpens;
};
