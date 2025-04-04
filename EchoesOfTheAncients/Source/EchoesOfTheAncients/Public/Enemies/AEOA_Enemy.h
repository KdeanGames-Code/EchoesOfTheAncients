// Copyright © 2025 Kdean Games. All Rights Reserved
// Defines the AEnemy class, a base class for enemies in Echoes of the Ancients.
// Enemies are hittable targets for Aria’s attacks, configured to interact with 
// weapon box traces.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/HitInterface.h"
#include "AEOA_Enemy.generated.h"

// Forward declarations to minimize header dependencies
class UAnimMontage;
class UParticleSystem;
class USoundBase;

UCLASS()
class ECHOESOFTHEANCIENTS_API AAEOA_Enemy : public ACharacter, public IHitInterface
{
	GENERATED_BODY()

public:
	/// Constructor for AEnemy, initializes enemy properties and components.
	AAEOA_Enemy();

	/// Called every frame, updates enemy state.
	virtual void Tick(float DeltaTime) override;

	/// Called to bind functionality to input (not used for AI-controlled enemies, but required override).
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/// Handles the reaction when the enemy is hit by a weapon.
	/// @param ImpactPoint The location where the enemy was hit.
	virtual void GetHit_Implementation(const FVector& ImpactPoint) override;

	void DirectionalHitReact(const FVector& ImpactPoint);

private:

	/**
	 * Animation montages
	 */
	 /// Animation montage for hit reactions, editable in the default Blueprint.
	UPROPERTY(EditDefaultsOnly, Category = "Montages",
		meta = (ToolTip = "Animation montage containing hit react animations for the enemy, set in the default Blueprint."))
	UAnimMontage* HitReactMontage;

	/// Sound to play when the enemy is hit, editable in Blueprints.
	UPROPERTY(EditAnywhere, Category = "Sounds",
		meta = (ToolTip = "Sound to play when the enemy is hit, set in the default Blueprint or on individual instances."))
	USoundBase* HitSound;

	/// Particle system to spawn when the enemy is hit, editable in Blueprints.
	UPROPERTY(EditAnywhere, Category = "VisualEffects",
		meta = (ToolTip = "Particle system to spawn when the enemy is hit, set in the default Blueprint or on individual instances."))
	UParticleSystem* HitParticles;


protected:

	/// Called when the game starts or when spawned, initializes enemy behavior.
	virtual void BeginPlay() override;

	/**
	 * Play montage functions
	 */
	 /// Plays the hit react montage with the specified section.
	 /// @param SectionName The name of the section to play (e.g., "FromLeft", "FromRight").
	void PlayHitReactMontage(const FName& SectionName);

public:
	// Getters and setters

};
