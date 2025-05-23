// Copyright © 2025 Kdean Games. All Rights Reserved


#include "Items/AEOA_Item.h"
#include "Characters/AriaCharacter.h"
#include "Components/SphereComponent.h"
#include "EchoesOfTheAncients/DebugMacros.h"
#include "Math/UnrealMathUtility.h"
#include "NiagaraComponent.h"

// --- Constructor ---
AAEOA_Item::AAEOA_Item()
{
	// Enable ticking for this actor
	PrimaryActorTick.bCanEverTick = true;

	// Create and initialize the static mesh component as a default subobject
	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMeshComponent"));
	// Set ItemMesh as the root component, replacing the default scene root
	RootComponent = ItemMesh;

	// Create a sphere component for overlap detection and attach it to the root component.
	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Sphere->SetupAttachment(GetRootComponent());

	SpectralxAIEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("SpectralxAIEffect"));
	SpectralxAIEffect->SetupAttachment(GetRootComponent());
}

// --- Begin Play ---
void AAEOA_Item::BeginPlay()
{
	Super::BeginPlay();
	
	// Bind the OnSphereOverlap callback to the OnComponentBeginOverlap delegate on the Sphere component.
	// This ensures that OnSphereOverlap is called whenever another actor overlaps with the Sphere.
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AAEOA_Item::OnSphereOverlap);

	// Bind the OnSphereEndOverlap function to the OnComponentEndOverlap delegate on the Sphere component.
	// AddDynamic allows runtime binding of the callback to handle the end of an overlap event.
	Sphere->OnComponentEndOverlap.AddDynamic(this, &AAEOA_Item::OnSphereEndOverlap);
}

// --- Transformed Functions ---
// Computes a sine wave value for vertical bobbing motion based on RunningTime and TimeConstant.
float AAEOA_Item::TransformedSin()
{
	return Amplitude * FMath::Sin(RunningTime * TimeConstant);  // Compute transformed sine
}

// Computes a cosine wave value for potential horizontal motion based on RunningTime and TimeConstant.
float AAEOA_Item::TransformedCos()
{
	return Amplitude * FMath::Cos(RunningTime * TimeConstant);  // Compute transformed cosine
}

// --- Overlap Begin Callback ---
// Callback function triggered when the Sphere component overlaps with another actor.
void AAEOA_Item::OnSphereOverlap(
	UPrimitiveComponent* OverlappedComponent,   // The component on this actor that was overlapped (Sphere)
	AActor* OtherActor,                         // The actor that overlapped with this component (e.g., player)
	UPrimitiveComponent* OtherComp,             // The component on the other actor (e.g., player's capsule)
	int32 OtherBodyIndex,                       // Index of the physics body on the other actor (if applicable)
	bool bFromSweep,                            // True if the overlap resulted from a sweep (movement check)
	const FHitResult& SweepResult               // Details about the overlap point if bFromSweep is true
	)
{
	//// Cast the overlapping actor to AAriaCharacter (Aria) to set the overlapping item.
	AAriaCharacter* AriaCharacter = Cast<AAriaCharacter>(OtherActor);
	if (AriaCharacter)
	{
		AriaCharacter->SetOverlappingItem(this); // Set this item as the overlapping item on Aria.
	}
}

// --- Overlap End Callback ---
// Triggered when the Sphere component stops overlapping with another actor, printing a message with the actor's name in blue.
void AAEOA_Item::OnSphereEndOverlap(
	UPrimitiveComponent* OverlappedComponent,   // The Sphere component on this actor that stopped overlapping
	AActor* OtherActor,                         // The actor that stopped overlapping (e.g., player)
	UPrimitiveComponent* OtherComp,             // The component on the other actor (e.g., player's capsule)
	int32 OtherBodyIndex                        // Index of the physics body on the other actor (if applicable)
	)
{
	// Cast the overlapping actor to AAriaCharacter (Aria) to clear the overlapping item.
	AAriaCharacter* AriaCharacter = Cast<AAriaCharacter>(OtherActor);
	if (AriaCharacter)
	{
		AriaCharacter->SetOverlappingItem(nullptr);  // Clear the overlapping item when Aria stops overlapping.
	}
}

void AAEOA_Item::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Increment RunningTime to drive sine-based bobbing and rotation effects over time.
	RunningTime += DeltaTime;

	// Apply vertical bobbing motion to the Z-axis using the transformed sine value, only if the item is in the hovering state.
	if (ItemState == EItemState::EIS_Hovering)
	{
		// Apply vertical bobbing motion to the Z-axis using the transformed sine value.
		AddActorWorldOffset(FVector(0.f, 0.f, TransformedSin()));
		// Apply rotation around the Y-axis, scaled by DeltaTime for frame-rate independence.
		AddActorWorldRotation(FRotator(0.f, RotationRate * DeltaTime, 0.f));
	}
}