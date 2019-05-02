// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoor.h"
#include "Gameframework/Actor.h"

#define OUT

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
}

void UOpenDoor::OpenDoor()
{
    Owner->SetActorRotation(FRotator(0.f, 190.f, 0.f));
}

void UOpenDoor::CloseDoor()
{
    Owner->SetActorRotation(FRotator(0.f, 270.f, 0.f));
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    
    if (GetTotalMassOfActorsOnPlate() > 30.f) {
        OpenDoor();
        DoorLastOpenTime = GetWorld()->GetTimeSeconds();
    }
    
    if (GetWorld()->GetTimeSeconds() - DoorLastOpenTime > DoorCloseDelay) {
        CloseDoor();
    }

}

float UOpenDoor::GetTotalMassOfActorsOnPlate() const
{
    float TotalMass = 0.f;
    TArray<AActor*> OverlappingActors;
    if (!PressurePlate) { return TotalMass; }
    
    PressurePlate->GetOverlappingActors(OUT OverlappingActors);
    
    for (const auto& Actor : OverlappingActors) {
        TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
    }
    
    return TotalMass;
}
