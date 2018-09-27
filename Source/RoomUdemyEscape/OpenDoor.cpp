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


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// If the total mass of actor is greater than the value, the door opens		
	if ( GetTotalMassOfActorsOnPlate() > OpenThresholdMass)
	{		
		OnOpen.Broadcast();
	}	
	else
	{
		OnClose.Broadcast();	
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
		UE_LOG(LogTemp, Error, TEXT("No pressure plate."));
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

