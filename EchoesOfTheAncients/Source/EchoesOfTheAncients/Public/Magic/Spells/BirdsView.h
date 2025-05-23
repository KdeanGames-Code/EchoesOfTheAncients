// Copyright © 2025 Kdean Games. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "InputActionValue.h"
#include "BirdsView.generated.h"

// Forward declarations to minimize header dependencies
class UCapsuleComponent;
class USkeletalMeshComponent;
class UFloatingPawnMovement;
class UInputMappingContext;
class UInputAction;
class USpringArmComponent;
class UCameraComponent;

UCLASS()
class ECHOESOFTHEANCIENTS_API ABirdsView : public APawn
{
	GENERATED_BODY()

public:
	/// Constructor for ABirdsView, initializes pawn properties.
	ABirdsView();

	/// Called every frame, updates pawn state.
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	/// Called when the game starts or when spawned.
	virtual void BeginPlay() override;

	/// Moves the bird based on input action value.
	UFUNCTION()
	void BirdMove(const FInputActionValue& Value);

	/// Rotates the bird left/right based on mouse X input.
	UFUNCTION()
	void Turn(const FInputActionValue& Value);

	/// Adjusts the bird’s pitch up/down based on mouse Y input.
	UFUNCTION()
	void LookUp(const FInputActionValue& Value);

	// Input mapping context for bird movement, editable in Blueprints.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Birds View|Input",
		meta = (ToolTip = "Input mapping context defining key bindings for bird movement (e.g., W key for forward)."))
	UInputMappingContext* IMC_BirdsViewMapping;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Birds View|Input",
		meta = (ToolTip = "Input action for moving the bird, typically bound to W key for forward movement."))
	UInputAction* BirdMoveAction;

	/// Input action for turning the bird, editable in Blueprints.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Birds View|Input",
		meta = (ToolTip = "Input action for turning the bird left or right, bound to MouseX."))
	UInputAction* TurnAction;

	/// Input action for looking up/down, editable in Blueprints.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Birds View|Input",
		meta = (ToolTip = "Input action for adjusting the bird’s pitch, bound to MouseY with inverted scale."))
	UInputAction* LookUpAction;

private:	
	
	/// Capsule component for basic collision detection, visible in Blueprints.
	UPROPERTY(VisibleAnywhere, Category = "Birds View|Collision",
		meta = (ToolTip = "Capsule component used for collision detection, sized for bird-like dimensions."))
	UCapsuleComponent* BirdsViewCapsule;

	/// Skeletal mesh component for the bird’s visual representation, visible in Blueprints.
	UPROPERTY(VisibleAnywhere, Category = "Birds View|Visual",
		meta = (ToolTip = "Skeletal mesh component for the bird's animated visual representation (e.g., crow mesh)."))
	USkeletalMeshComponent* BirdsViewMesh;

	/// Movement component for floating pawn motion, visible in Blueprints.
	UPROPERTY(VisibleAnywhere, Category = "Birds View|Movement",
		meta = (ToolTip = "Floating pawn movement component for controlling bird motion with input bindings."))
	UFloatingPawnMovement* BirdsViewMovement;

	/// Spring arm component to offset and manage the camera, visible in Blueprints.
	UPROPERTY(VisibleAnywhere, Category = "Birds View|Camera",
		meta = (ToolTip = "Spring arm component to offset the camera, enabling dynamic distance adjustment."))
	USpringArmComponent* CameraBoom;

	/// Camera component for the player’s viewpoint, visible in Blueprints.
	UPROPERTY(VisibleAnywhere, Category = "Birds View|Camera",
		meta = (ToolTip = "Camera component providing the player’s viewpoint, attached to the spring arm."))
	UCameraComponent* ViewCamera;
};
