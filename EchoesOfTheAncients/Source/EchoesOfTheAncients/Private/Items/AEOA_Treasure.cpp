// Copyright © 2025 Kdean Games. All Rights Reserved
// Implements the AEOA_Treasure class, handling treasure pickup logic in Echoes of the Ancients.

#include "Items/AEOA_Treasure.h"
#include "Characters/AriaCharacter.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

void AAEOA_Treasure::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    AAriaCharacter* AriaCharacter = Cast<AAriaCharacter>(OtherActor);
    if (AriaCharacter)
    {
        if (PickupSound)
        {
            UGameplayStatics::PlaySoundAtLocation(
                this,
                PickupSound,
                GetActorLocation()
            );
            if (IsDawnspire)
            {
                AriaCharacter->IncrementDawnspireCount();
            }
        }
        Destroy();
    }
}
