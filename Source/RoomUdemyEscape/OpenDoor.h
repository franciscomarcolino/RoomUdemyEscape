// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerVolume.h"
#include "Components/ActorComponent.h"
#include "Runtime/Core/Public/Containers/Array.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Runtime/Engine/Classes/Components/PrimitiveComponent.h"
#include "OpenDoor.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ROOMUDEMYESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void OpenDoor();

	void CloseDoor();

	float GetTotalMassOfActorsOnPlate();

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:

	UPROPERTY(VisibleAnyWhere)
	float DoorOpenAngle = -90.0f;

	UPROPERTY(VisibleAnyWhere)
	float DoorCloseAngle = 0.0f;

	UPROPERTY(VisibleAnyWhere)
	bool AvaiableToOpen = true;

	UPROPERTY(VisibleAnyWhere)
	float DoorCloseDelay = 0.5f;

	UPROPERTY(VisibleAnyWhere)
	float LastDoorOpenTime;
	float OpenThresholdMass = 40.0f;

	UPROPERTY(EditAnyWhere)
	ATriggerVolume* PressurePlate;


};