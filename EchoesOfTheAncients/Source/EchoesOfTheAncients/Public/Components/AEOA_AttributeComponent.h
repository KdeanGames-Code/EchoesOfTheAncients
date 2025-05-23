// Copyright © 2025 Kdean Games. All Rights Reserved
// Defines the UAEOA_AttributeComponent class, a component for 
// managing actor attributes like health in Echoes of the Ancients.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AEOA_AttributeComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ECHOESOFTHEANCIENTS_API UAEOA_AttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	
	UAEOA_AttributeComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/// Checks if the actor is alive based on health.
	/// @return True if health is greater than 0, false otherwise.
	bool IsAlive();

protected:
	
	virtual void BeginPlay() override;

private:	
	
	/// Current health of the actor.
	UPROPERTY(EditAnywhere, Category = "Actor Attributes",
		meta = (ToolTip = "Current health of the actor, editable in Blueprints or instances."))
	float Health = 100.f;

	/// Maximum health of the actor.
	UPROPERTY(EditAnywhere, Category = "Actor Attributes",
		meta = (ToolTip = "Maximum health of the actor, editable in Blueprints or instances."))
	float MaxHealth = 100.f;

public:

	/// Reduces the actor’s health by the specified damage amount, clamping between 0 and MaxHealth.
	/// @param Damage The amount of damage to apply to the actor’s health.
	void ReceiveDamage(float Damage);

	/// Calculates the current health percentage as a fraction of MaxHealth.
	/// @return The health percentage (0.0 to 1.0).
	float GetHealthPercent();
};
