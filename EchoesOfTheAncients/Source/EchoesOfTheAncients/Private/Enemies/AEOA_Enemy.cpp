// Copyright © 2025 Kdean Games. All Rights Reserved
// Implements the AEnemy class, configuring collision 
// properties for enemies in Echoes of the Ancients.

#include "Enemies/AEOA_Enemy.h"
#include "Animation/AnimMontage.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EchoesOfTheAncients/DebugMacros.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"

// --- Constructor ---
// Sets default values for the enemy’s properties and components.
AAEOA_Enemy::AAEOA_Enemy()
{
	PrimaryActorTick.bCanEverTick = true;

	// Configure the skeletal mesh component for collision and hit detection.
	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);  // Set object type to WorldDynamic to be hit by weapon box trace.
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);  // Block Visibility channel for box trace hits.
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);  // Ignore Camera channel to prevent camera collision issues.
	GetMesh()->SetGenerateOverlapEvents(true);  // Enable overlap events to trigger weapon box overlaps.

	// Configure the capsule component to ignore camera collision.
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

}

// --- BeginPlay ---
// Called when the game starts or when spawned.
void AAEOA_Enemy::BeginPlay()
{
	Super::BeginPlay();
	
}

void AAEOA_Enemy::PlayHitReactMontage(const FName& SectionName)
{
	// Get the animation instance from the enemy’s mesh.
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	// Check if the anim instance and hit react montage are valid before proceeding.
	if (AnimInstance && HitReactMontage)
	{
		// Play the hit react montage with default settings.
		AnimInstance->Montage_Play(HitReactMontage);
		// Jump to the specified section in the hit react montage.
		AnimInstance->Montage_JumpToSection(SectionName, HitReactMontage);
	}
}

// --- Tick ---
// Called every frame.
void AAEOA_Enemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// --- SetupPlayerInputComponent ---
// Called to bind functionality to input (not used for AI-controlled enemies).
void AAEOA_Enemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

// --- GetHit ---
// Handles the reaction when the enemy is hit by a weapon, 
// drawing a debug sphere at the impact point.
void AAEOA_Enemy::GetHit_Implementation(const FVector& ImpactPoint)
{
	// DRAW_SPHERE_COLOR(ImpactPoint, FColor::Orange);  // Draw an orange sphere at the impact point for 5 seconds.

	DirectionalHitReact(ImpactPoint);

	// Play the hit sound at the impact point if set.
	if (HitSound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			this,
			HitSound,
			ImpactPoint
		);
	}

	// Spawn the hit particles at the impact point if set.
	if (HitParticles && GetWorld())
	{
		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			HitParticles,
			ImpactPoint
		);
	}
}

void AAEOA_Enemy::DirectionalHitReact(const FVector& ImpactPoint)
{
	// Calculate the enemy's forward vector.
	const FVector Forward = GetActorForwardVector();

	// Lower the impact point to the enemy's Z level for a more accurate angle calculation.
	const FVector ImpactLowered(ImpactPoint.X, ImpactPoint.Y, GetActorLocation().Z);
	// Calculate the vector from the enemy's location to the lowered impact point (normalized).
	const FVector ToHit = (ImpactLowered - GetActorLocation()).GetSafeNormal();

	// Calculate the angle between Forward and ToHit using the dot product.
	// Forward * ToHit = |Forward||ToHit| * cos(theta)
	// |Forward| = 1, |ToHit| = 1, so Forward * ToHit = cos(theta)
	const double CosTheta = FVector::DotProduct(Forward, ToHit);

	// Take the inverse cosine (arc-cosine) of cos(theta) to get theta in radians.
	double Theta = FMath::Acos(CosTheta);
	// Convert from radians to degrees.
	Theta = FMath::RadiansToDegrees(Theta);

	// Use the cross product to determine the sign of Theta (left or right).
	// If CrossProduct points down (Z < 0), Theta should be negative (hit from the left).
	const FVector CrossProduct = FVector::CrossProduct(Forward, ToHit);



	if (CrossProduct.Z < 0)
	{
		Theta *= -1.f;
	}

	// Determine the montage section based on Theta.
	FName Section("FromBack");  // Default to FromBack if none of the conditions match.
	if (Theta >= -45.f && Theta < 45.f)
	{
		Section = FName("FromFront");  // Hit from the front, stumble backward.
	}
	else if (Theta >= -135.f && Theta < -45.f)
	{
		Section = FName("FromLeft");  // Hit from the left, stumble right.
	}
	else if (Theta >= 45.f && Theta < 135.f)
	{
		Section = FName("FromRight");  // Hit from the right, stumble left.
	}
	// If Theta is >= 135 or < -135, Section remains "FromBack" (hit from behind, stumble forward).

	// Debug: Draw the cross product vector as a blue arrow.
	// UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + CrossProduct * 100.f, 5.f, FColor::Blue, 5.f);

	/*
	// Debug: Display the angle on screen.
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Green, FString::Printf(TEXT("Theta: %f"), Theta));
	}

	// Debug: Draw arrows to visualize the Forward and ToHit vectors.
	UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + Forward * 60.f, 5.f, FColor::Red, 5.f);
	UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + ToHit * 60.f, 5.f, FColor::Green, 5.f);
	*/

	PlayHitReactMontage(FName(Section));
}
