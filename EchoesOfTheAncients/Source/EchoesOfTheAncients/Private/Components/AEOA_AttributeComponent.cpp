// Copyright © 2025 Kdean Games. All Rights Reserved
// Implements the UAEOA_AttributeComponent class, 
// managing actor attributes like health in Echoes of the Ancients.

#include "Components/AEOA_AttributeComponent.h"

// Sets default values for this component's properties
UAEOA_AttributeComponent::UAEOA_AttributeComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UAEOA_AttributeComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UAEOA_AttributeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool UAEOA_AttributeComponent::IsAlive()
{
	return Health > 0.f;
}

void UAEOA_AttributeComponent::ReceiveDamage(float Damage)
{
	Health = FMath::Clamp(Health - Damage, 0.f, MaxHealth);
}

float UAEOA_AttributeComponent::GetHealthPercent()
{
	return Health / MaxHealth;
}

