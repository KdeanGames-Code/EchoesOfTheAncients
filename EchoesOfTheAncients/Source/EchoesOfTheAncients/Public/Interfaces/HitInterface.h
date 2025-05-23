// Copyright © 2025 Kdean Games. All Rights Reserved
// Defines the IHitInterface, an Unreal Engine interface 
// for handling hit reactions in Echoes of the Ancients.
// Classes implementing this interface can define custom behavior when hit by a weapon.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "HitInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UHitInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * Interface for actors that can be hit by a weapon, 
   providing a GetHit function to define custom hit reactions.
 */
class ECHOESOFTHEANCIENTS_API IHitInterface
{
	GENERATED_BODY()

public:

	/// Pure virtual function to be implemented by classes that can be hit.
	/// Defines the reaction when the actor is hit by a weapon (e.g., take damage, break).
	/// @param ImpactPoint The location where the actor was hit.
	UFUNCTION(BlueprintNativeEvent)
	void GetHit(const FVector& ImpactPoint);
};
