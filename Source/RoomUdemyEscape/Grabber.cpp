// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "Grabber.h"

#define OUT

// Constructor, default values
UGrabber::UGrabber()
{	
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	FindPhysicsHandleComponent();
	SetupInputComponent();	

}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	// Draw the debug line
	DrawDebugLineInColor();

	FVector LineTraceEnd = GetLineTraceEnd(GetPlayerLocationRotation());

	//if Physics handle is attached
	if (PhysicsHandleComponent->GrabbedComponent)
	{
		// move the object we are holding
		PhysicsHandleComponent->SetTargetLocation(LineTraceEnd);
	}
	
}

// Check if the Owner of this script has the PhysicsHandleComponent that allow us to grab other objects
void UGrabber::FindPhysicsHandleComponent() 
{
	PhysicsHandleComponent = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();

	if (PhysicsHandleComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("The actor %s does have a PhysicsHandleComponent. "), *(GetOwner()->GetName()));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("The actor %s does not have a PhysicsHandleComponent. "), *(GetOwner()->GetName()));
	}
}

//Setup wich function will be called for the binded action in the project, in this case, the grab action
void UGrabber::SetupInputComponent() 
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();

	if (InputComponent)
	{
		//Log if the Owner have a input component
		UE_LOG(LogTemp, Warning, TEXT("The actor %s have a UInputComponent. "), *(GetOwner()->GetName()));

		//Setup of the called functions, Grab when the button is pressed and Release when the same button is released.
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else
	{
		//Log if the Owner does not have a input component
		UE_LOG(LogTemp, Error, TEXT("The actor %s does not have a UInputComponent. "), *(GetOwner()->GetName()));
	}
}

//Check if the object in range of the line trace is compatible, if yes, return the hit object. In this case the compatible objects are the CC_PhysicsBody.
FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	// Set the variables that will be modified and will be the out of the function GetPlayerViewPoint
	PlayerLocationRotation LocationRotation = GetPlayerLocationRotation();
	
	// Variable that stores the end of the linetrace used to debug
	FVector LineTraceEnd = GetLineTraceEnd(LocationRotation);
	
	// Variable that stores the hit result of the line trace
	FHitResult LineTraceHit;
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());

	// Real line trace, not the debug one above that is only visual, this one generates the collision events, in this case only for objets that are of the type ECC_PhysicsBody
	GetWorld()->LineTraceSingleByObjectType
	(
		OUT LineTraceHit,
		LocationRotation.PlayerViewpointLocation,
		LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParameters
	);

	// Variable to store the actors that are hit by the owner line trace
	AActor* ActorHit = LineTraceHit.GetActor();

	if (ActorHit)
	{
		// Message for loggin purposes
		UE_LOG(LogTemp, Warning, TEXT("The object %s was hit. "), *(ActorHit->GetName()));
	}

	return LineTraceHit;
}

//Function used to grab the objects
void UGrabber::Grab()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab Key pressed. "));
	auto HitResult = GetFirstPhysicsBodyInReach();
	auto ComponentToGrab = HitResult.GetComponent();
	auto ActorHit = HitResult.GetActor();

	//Attach only if we hit a valid actor
	if (ActorHit != nullptr)
	{
		PhysicsHandleComponent->GrabComponent(ComponentToGrab, NAME_None, ActorHit->GetActorLocation(), true);
	}
}

//Function used to release the objects
void UGrabber::Release()
{
	PhysicsHandleComponent->ReleaseComponent();
	UE_LOG(LogTemp, Warning, TEXT("Grab Key released. "));
}

// Function to draw the debug line
void UGrabber::DrawDebugLineInColor()
{
	PlayerLocationRotation PlayerCoords;

	PlayerCoords = GetPlayerLocationRotation();

	FVector LineTraceEnd = GetLineTraceEnd(PlayerCoords);
	
	FColor DebugColor = FColor(255, 0, 0);

	// Used to draw a red trace to debug
	DrawDebugLine
	(
		GetWorld(),
		PlayerCoords. PlayerViewpointLocation,
		LineTraceEnd,
		DebugColor,
		false,
		0.0f,
		0.0f,
		10.0f
	);	
}

PlayerLocationRotation UGrabber::GetPlayerLocationRotation()
{
	// Set the variables that will be modified and will be the out of the function GetPlayerViewPoint
	FVector PlayerViewpointLocation;
	FRotator PlayerViewpointRotation;
	PlayerLocationRotation ReturnStruct;

	//Function that gets the viewpoint
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewpointLocation, OUT PlayerViewpointRotation);

	ReturnStruct.PlayerViewpointLocation = PlayerViewpointLocation;
	ReturnStruct.PlayerViewpointRotation = PlayerViewpointRotation;

	return ReturnStruct;
}

FVector UGrabber::GetLineTraceEnd(PlayerLocationRotation LocationRotation)
{	

	//Calls the function to get the location and rotation, necessary to calculate the line trace end.
	LocationRotation = GetPlayerLocationRotation();
	
	// Variable that stores the end of the linetrace used to debug
	FVector LineTraceEnd = LocationRotation.PlayerViewpointLocation + (LocationRotation.PlayerViewpointRotation.Vector() * TraceReach);

	return LineTraceEnd;
}


