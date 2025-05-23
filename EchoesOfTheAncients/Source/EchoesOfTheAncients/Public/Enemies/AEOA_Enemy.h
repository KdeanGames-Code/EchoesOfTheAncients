// Copyright © 2025 Kdean Games. All Rights Reserved
// Defines the AEnemy class, a base class for enemies in Echoes of the Ancients.
// Enemies are hittable targets for Aria’s attacks, configured to interact with 
// weapon box traces.

#pragma once

#include "CoreMinimal.h"
#include "Characters/CharacterTypes.h"
#include "GameFramework/Character.h"
#include "Interfaces/HitInterface.h"
#include "AEOA_Enemy.generated.h"

// Forward declarations to minimize header dependencie
class UAEOA_AttributeComponent;
class UAEOA_HealthBarComponent;
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

	/// Handles damage application, reducing health and updating the health bar.
	/// @param DamageAmount The amount of damage to apply.
	/// @param DamageEvent The damage event data.
	/// @param EventInstigator The controller responsible for the damage.
	/// @param DamageCauser The actor causing the damage.
	/// @return The amount of damage actually applied.
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

private:

	/// Component for managing the enemy’s attributes, such as health.
	UPROPERTY(VisibleAnywhere)
	UAEOA_AttributeComponent* Attributes;

	/// Custom widget component to display the health bar above the enemy’s head in screen space.
	UPROPERTY(VisibleAnywhere)
	UAEOA_HealthBarComponent* HealthBarWidget;

	/**
	 * Animation montages
	 */
	 /// Animation montage for hit reactions, editable in the default Blueprint.
	UPROPERTY(EditDefaultsOnly, Category = "Montages",
		meta = (ToolTip = "Animation montage containing hit react animations for the enemy, set in the default Blueprint."))
	UAnimMontage* HitReactMontage;

	/// Animation montage for death animations, editable in the default Blueprint.
	UPROPERTY(EditDefaultsOnly, Category = "Montages",
		meta = (ToolTip = "Animation montage containing death animations for the enemy, set in the default Blueprint."))
	UAnimMontage* DeathMontage;

	/// Sound to play when the enemy is hit, editable in Blueprints.
	UPROPERTY(EditAnywhere, Category = "Sounds",
		meta = (ToolTip = "Sound to play when the enemy is hit, set in the default Blueprint or on individual instances."))
	USoundBase* HitSound;

	/// Particle system to spawn when the enemy is hit, editable in Blueprints.
	UPROPERTY(EditAnywhere, Category = "VisualEffects",
		meta = (ToolTip = "Particle system to spawn when the enemy is hit, set in the default Blueprint or on individual instances."))
	UParticleSystem* HitParticles;

	/// The current combat target (e.g., Aria), set when the enemy takes damage, used to manage health bar visibility.
	UPROPERTY()
	AActor* CombatTarget;

	/// The radius within which the enemy considers itself in combat, controlling health bar visibility.
	UPROPERTY(EditAnywhere, meta = (ToolTip = "The radius (in units) within which the enemy considers itself in combat, showing the health bar if the target is within this range."))
	double CombatRadius = 500.f;

protected:

	/// Called when the game starts or when spawned, initializes enemy behavior.
	virtual void BeginPlay() override;

	/// Plays a random death animation when the enemy’s health reaches zero.
	void Die();

	/**
	 * Play montage functions
	 */
	 /// Plays the hit react montage with the specified section.
	 /// @param SectionName The name of the section to play (e.g., "FromLeft", "FromRight").
	void PlayHitReactMontage(const FName& SectionName);

	/// The current death pose of the enemy, used to determine the final pose after death animation.
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	EDeathPose DeathPose = EDeathPose::EDP_Alive;

public:
	// Getters and setters

};
