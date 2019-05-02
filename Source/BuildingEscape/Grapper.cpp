// Fill out your copyright notice in the Description page of Project Settings.


#include "Grapper.h"
#include "DrawDebugHelpers.h"

// Sets default values for this component's properties
UGrapper::UGrapper()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrapper::BeginPlay()
{
	Super::BeginPlay();

	/// Look for attached Physics Handle
    PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
    
    /// Look for attached Input Component
    InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
    if (InputComponent) {
        InputComponent->BindAction("Grab", IE_Pressed, this, &UGrapper::Grab);
        InputComponent->BindAction("Grab", IE_Released, this, &UGrapper::Release);
    }

}


// Called every frame
void UGrapper::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    
    if (PhysicsHandle->GrabbedComponent) {
        PhysicsHandle->SetTargetLocation(GetReachLineEnd().v2);
    }
}

void UGrapper::Grab()
{
    FHitResult HitResult = GetFirstPhysicsBodyInReach();
    UPrimitiveComponent* ComponentToGrab = HitResult.GetComponent();
    AActor* ActorHit = HitResult.GetActor();

    if (ActorHit) {
        FVector PhysicsHandleLocation;
        FRotator PhysicsHandleRotation;
        PhysicsHandle->GetTargetLocationAndRotation(PhysicsHandleLocation, PhysicsHandleRotation);
        PhysicsHandle->GrabComponentAtLocationWithRotation(ComponentToGrab, NAME_None, ComponentToGrab->GetOwner()->GetActorLocation(), PhysicsHandleRotation);
    }
}

void UGrapper::Release()
{
    PhysicsHandle->ReleaseComponent();
}

FHitResult UGrapper::GetFirstPhysicsBodyInReach() const
{
    FTwoVectors TracePoints = GetReachLineEnd();
    
    FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());
    
    FHitResult Hit;
    GetWorld()->LineTraceSingleByObjectType(OUT Hit, TracePoints.v1, TracePoints.v2, FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody), TraceParameters);
    
    return Hit;
}

FTwoVectors UGrapper::GetReachLineEnd() const {
    FVector LineTraceBegin;
    FRotator PlayerViewPointRotation;
    
    GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT LineTraceBegin, OUT PlayerViewPointRotation);
    
    FVector LineTraceEnd = LineTraceBegin + PlayerViewPointRotation.Vector() * Reach;
    
    return FTwoVectors(LineTraceBegin, LineTraceEnd);
}
