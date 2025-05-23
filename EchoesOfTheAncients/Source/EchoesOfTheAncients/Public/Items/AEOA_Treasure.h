// Copyright © 2025 Kdean Games. All Rights Reserved
// Defines the AEOA_Treasure class, a child of AEOA_Item, 
// representing treasure pickups in Echoes of the Ancients.
// Plays a pickup sound and destroys itself when overlapped by the player character.

#pragma once

#include "CoreMinimal.h"
#include "Items/AEOA_Item.h"
#include "AEOA_Treasure.generated.h"

class USoundBase;

/**
 * 
 */
UCLASS()
class ECHOESOFTHEANCIENTS_API AAEOA_Treasure : public AAEOA_Item
{
	GENERATED_BODY()
	
protected:
    /// Called when the Sphere component begins overlapping with another actor, 
    /// plays a pickup sound and destroys the treasure.
    virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

private:
    UPROPERTY(EditAnywhere, Category = "Sounds",
        meta = (ToolTip = "Sound to play when the treasure is picked up, set in the default Blueprint (e.g., a coin rustling sound)."))
    USoundBase* PickupSound;

    /// Identifies if this treasure is a Dawnspire coin, used for collection counting in AAriaCharacter.
    UPROPERTY(EditAnywhere, Category = "Treasure Properties",
        meta = (ToolTip = "Identifies if this treasure is a Dawnspire coin, used for collection counting."))
    bool IsDawnspire;

};
