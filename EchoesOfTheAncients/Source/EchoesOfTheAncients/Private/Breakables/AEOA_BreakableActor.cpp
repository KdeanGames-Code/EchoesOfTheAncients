// Copyright © 2025 Kdean Games. All Rights Reserved
// Implements the AEOA_BreakableActor class, handling 
// destruction logic for breakable objects in Echoes of the Ancients.

#include "Breakables/AEOA_BreakableActor.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "Items/AEOA_Treasure.h"
#include "Components/CapsuleComponent.h"

AAEOA_BreakableActor::AAEOA_BreakableActor()
{
	PrimaryActorTick.bCanEverTick = false;

	GeometryCollection = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("GeometryCollection"));
	SetRootComponent(GeometryCollection);
	GeometryCollection->SetGenerateOverlapEvents(true);
	GeometryCollection->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GeometryCollection->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);

	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	Capsule->SetupAttachment(GetRootComponent());
	Capsule->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	Capsule->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
}

void AAEOA_BreakableActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void AAEOA_BreakableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAEOA_BreakableActor::GetHit_Implementation(const FVector& ImpactPoint)
{
	if (bBroken) return;
	bBroken = true;

	UWorld* World = GetWorld();
	if (World && TreasureClass)
	{
		FVector Location = GetActorLocation();
		Location.Z += 75.f;
		World->SpawnActor<AAEOA_Treasure>(TreasureClass, Location, GetActorRotation());
	}
}
