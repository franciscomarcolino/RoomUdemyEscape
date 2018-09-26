// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "OpenDoor.h"


#define OUT

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{	
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();	
}

void UOpenDoor::OpenDoor()
{
	// Find the owning actor
	AActor* Owner = GetOwner();

	// Set the new rotation
	Owner->SetActorRotation(FRotator(0.0f, DoorOpenAngle, 0.0f));
}

void UOpenDoor::CloseDoor()
{
	// Find the owning actor
	AActor* Owner = GetOwner();

	// Set the new rotation
	Owner->SetActorRotation(FRotator(0.0f, DoorCloseAngle, 0.0f));
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Poll the trigger every frame
	if ( GetTotalMassOfActorsOnPlate() > OpenThresholdMass)
	{	
	// If the total mass of actor is greater than the value, the door opens		
		OpenDoor();
		AvaiableToOpen = false;
		LastDoorOpenTime = GetWorld()->GetTimeSeconds();
	}	
	else if ( (GetWorld()->GetTimeSeconds() - LastDoorOpenTime) > DoorCloseDelay)
	{
		CloseDoor();
		AvaiableToOpen = true;
	}
}

float UOpenDoor::GetTotalMassOfActorsOnPlate()
{
	//Initialization of total mass of actors
	float TotalMass = 0.0f;
	// Find all overlaping actors
	TArray<AActor*> OverlappingActors;
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);

	// Iterate to sum the masses

	for (auto* Actor : OverlappingActors)
	{ 	
		UE_LOG(LogTemp, Warning, TEXT("Object overlaping now: %s "), *(Actor->GetName()));
		TotalMass = TotalMass + Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();		
	}

	return TotalMass;
}

