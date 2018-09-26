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

	if (PressurePlate != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("The actor %s does have a PressurePlate. "), *(GetOwner()->GetName()));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("The actor %s does not have a PressurePlate. "), *(GetOwner()->GetName()));
	}

	return;
}

void UOpenDoor::OpenDoor()
{
	////// Find the owning actor
	//AActor* Owner = GetOwner();

	//if (!Owner) { return; }
	////// Set the new rotation
	//Owner->SetActorRotation(FRotator(0.0f, DoorOpenAngle, 0.0f));

	/*OnOpenRequest.Broadcast();*/

	//UE_LOG(LogTemp, Error, TEXT("Passou!"));

	return;	
}

void UOpenDoor::CloseDoor()
{
	//// Find the owning actor
	//AActor* Owner = GetOwner();
	//if (!Owner) { return; }
	//// Set the new rotation
	//Owner->SetActorRotation(FRotator(0.0f, DoorCloseAngle, 0.0f));



	return;
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Poll the trigger every frame
	if ( GetTotalMassOfActorsOnPlate() > OpenThresholdMass)
	{	
	// If the total mass of actor is greater than the value, the door opens		
		OnOpenRequest.Broadcast();
		/*AvaiableToOpen = false;
		LastDoorOpenTime = GetWorld()->GetTimeSeconds();*/
	}	
	else /*if ( (GetWorld()->GetTimeSeconds() - LastDoorOpenTime) > DoorCloseDelay)*/
	{
		FOnCloseRequest.Broadcast();
		/*CloseDoor();*/
		/*AvaiableToOpen = true;*/
	}

	return;
}

float UOpenDoor::GetTotalMassOfActorsOnPlate()
{
	//Initialization of total mass of actors
	float TotalMass = 0.0f;

	// Find all overlaping actors
	TArray<AActor*> OverlappingActors;

	if (PressurePlate == nullptr) 
	{
		UE_LOG(LogTemp, Error, TEXT("No mass"));
		return 0.0f; 
	}
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);

	// Iterate to sum the masses

	for (auto* Actor : OverlappingActors)
	{ 	
		UE_LOG(LogTemp, Warning, TEXT("Object overlaping now: %s "), *(Actor->GetName()));
		TotalMass = TotalMass + Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		UE_LOG(LogTemp, Warning, TEXT("Object overlaping now: %f "), TotalMass);
	}

	return TotalMass;
}

