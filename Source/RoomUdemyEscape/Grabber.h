// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Runtime/Engine/Classes/Components/InputComponent.h"
#include "Runtime/Engine/Classes/PhysicsEngine/PhysicsHandleComponent.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Runtime/Engine/Public/DrawDebugHelpers.h"
#include "Runtime/Engine/Classes/GameFramework/PlayerController.h"
#include "Grabber.generated.h"

//TODO Proteger todos os ponteiros

struct PlayerLocationRotation
{
	FVector PlayerViewpointLocation;
	FRotator PlayerViewpointRotation;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ROOMUDEMYESCAPE_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	
	float TraceReach = 100.0f;
		
	UPhysicsHandleComponent* PhysicsHandleComponent = nullptr;

	UInputComponent* InputComponent = nullptr;


	//*********** ACTIONS
		
		//Performs the grab action
		void Grab();	
		//Performs the release action
		void Release();
		//Draw the debug line
		void DrawDebugLineInColor();

	//*********** GETTERS

		//Get player location and rotation
		PlayerLocationRotation GetPlayerLocationRotation();

		//Get Line Trace end
		FVector GetLineTraceEnd(PlayerLocationRotation);

		// Find Attached physics
		void FindPhysicsHandleComponent();

		// Setup the input component
		void SetupInputComponent();

		// Return hit for the first body in reach
		FHitResult GetFirstPhysicsBodyInReach();

	
};
