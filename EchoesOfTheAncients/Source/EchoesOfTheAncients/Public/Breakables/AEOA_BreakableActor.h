// Copyright © 2025 Kdean Games. All Rights Reserved
// Defines the AEOA_BreakableActor class, an actor that can 
// be broken by Aria’s attacks in Echoes of the Ancients.
// Uses a Geometry Collection to manage destructible meshes.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/HitInterface.h"
#include "AEOA_BreakableActor.generated.h"

class UGeometryCollectionComponent;

UCLASS()
class ECHOESOFTHEANCIENTS_API AAEOA_BreakableActor : public AActor, public IHitInterface
{
	GENERATED_BODY()
	
public:

	/// Constructor for AEOA_BreakableActor, 
	//  initializes the GeometryCollectionComponent and collision settings.
	AAEOA_BreakableActor();

	virtual void Tick(float DeltaTime) override;

	// Handles the reaction when the actor is hit by a weapon
	virtual void GetHit_Implementation(const FVector& ImpactPoint) override;

protected:
	
	virtual void BeginPlay() override;

public:	
	
private:
	UPROPERTY(VisibleAnywhere)
	UGeometryCollectionComponent* GeometryCollection;

};
