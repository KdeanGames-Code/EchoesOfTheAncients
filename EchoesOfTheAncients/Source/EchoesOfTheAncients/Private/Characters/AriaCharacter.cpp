// Copyright © 2025 Kdean Games. All Rights Reserved
// Implements the AAriaCharacter class, handling Aria’s movement, camera, 
// and interactions in Echoes of the Ancients.

#include "Characters/AriaCharacter.h"
#include "Camera/CameraComponent.h"
#include "Characters/CharacterTypes.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GroomComponent.h"
#include "Items/AEOA_Item.h"
#include "Items/Weapons/AEOA_Weapon.h"

// Sets default values
AAriaCharacter::AAriaCharacter()
{
	// Enable ticking for this character
	PrimaryActorTick.bCanEverTick = true;

    // Disable controller rotation for character (camera will follow instead)
    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;

    // Configure character movement to orient to movement direction
    GetCharacterMovement()->bOrientRotationToMovement = true;  // Rotate character to face movement
    GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);  // Set rotation speed (yaw)
    GetCharacterMovement()->MaxWalkSpeed = 200.0f;  // Set walking speed for natural pace

    // Create and initialize the spring arm component for camera offset
    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(GetRootComponent());
    CameraBoom->TargetArmLength = 300.0f;
    CameraBoom->bUsePawnControlRotation = true;  // Camera follows controller rotation

    // Create and initialize the camera component for the viewpoint
    ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ViewCamera"));
    ViewCamera->SetupAttachment(CameraBoom);

    // Create and initialize the groom component for hair
    Hair = CreateDefaultSubobject<UGroomComponent>(TEXT("Hair"));
    Hair->SetupAttachment(GetMesh());
    Hair->AttachmentName = FString("head");  // Attach to head socket

    // Create and initialize the groom component for eyebrows
    Eyebrows = CreateDefaultSubobject<UGroomComponent>(TEXT("Eyebrows"));
    Eyebrows->SetupAttachment(GetMesh());
    Eyebrows->AttachmentName = FString("head");  // Attach to head socket
}

// --- Begin Play ---
void AAriaCharacter::BeginPlay()
{
	Super::BeginPlay();
	
    if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
    {
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
        {
            if (IMC_AriaMovement)
            {
                Subsystem->AddMappingContext(IMC_AriaMovement, 0);  // Add movement context
            }
        }
    }
}

// --- Move ---
// Moves Aria forward or backward based on the input action value (W/S keys).
void AAriaCharacter::Move(const FInputActionValue& Value)
{
    const float DirectionValue = Value.Get<float>();
    if (Controller && (DirectionValue != 0.0f))
    {
        // Use controller's forward vector based on its rotation
        const FRotator ControlRotation = Controller->GetControlRotation();
        const FRotationMatrix RotationMatrix(ControlRotation);
        const FVector Forward = RotationMatrix.GetUnitAxis(EAxis::X);  // Controller's forward direction
        AddMovementInput(Forward, DirectionValue);  // Move forward with W, backward with S
    }
}

// --- Move Right/Left ---
// Moves Aria left or right based on the input action value (A/D keys).
void AAriaCharacter::MoveRight(const FInputActionValue& Value)
{
    const float DirectionValue = Value.Get<float>();
    if (Controller && (DirectionValue != 0.0f))
    {
        // Use controller's right vector based on its rotation
        const FRotator ControlRotation = Controller->GetControlRotation();
        const FRotationMatrix RotationMatrix(ControlRotation);
        const FVector Right = RotationMatrix.GetUnitAxis(EAxis::Y);  // Controller's right directionAddMovementInput(Right, DirectionValue);
        AddMovementInput(Right, DirectionValue);
    }
}

// --- Turn (Yaw) ---
// Rotates Aria left or right based on mouse X input.
void AAriaCharacter::Turn(const FInputActionValue& Value)
{
    const float YawValue = Value.Get<FVector2D>().X;
    if (Controller)
    {
        AddControllerYawInput(YawValue);
    }
}

// --- Look Up (Pitch) ---
// Adjusts Aria’s pitch up or down based on mouse Y input.
void AAriaCharacter::LookUp(const FInputActionValue& Value)
{
    const float PitchValue = Value.Get<FVector2D>().Y;
    if (Controller)
    {
        AddControllerPitchInput(PitchValue);
    }
}

// --- Pickup Item ---
// Callback for the 'E' key press to equip an overlapping weapon.
void AAriaCharacter::PickupItem()
{
    AAEOA_Weapon* OverlappingWeapon = Cast<AAEOA_Weapon>(OverlappingItem);
    if (OverlappingWeapon)
    {
        OverlappingWeapon->Equip(GetMesh(), FName("R_hand_weapon")); // Equip the weapon to Aria’s right hand socket.
        CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;  // Update Aria’s state to reflect she’s equipped a one-handed weapon.
    }
}

// --- Tick ---
void AAriaCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// -- - Setup Player Input Component-- -
void AAriaCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

    if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
    {
        EnhancedInputComponent->BindAction(IA_MoveAction, ETriggerEvent::Triggered, this, &AAriaCharacter::Move);  // Bind movement action
        EnhancedInputComponent->BindAction(IA_MoveRightAction, ETriggerEvent::Triggered, this, &AAriaCharacter::MoveRight);
        EnhancedInputComponent->BindAction(IA_TurnAction, ETriggerEvent::Triggered, this, &AAriaCharacter::Turn);
        EnhancedInputComponent->BindAction(IA_LookUpAction, ETriggerEvent::Triggered, this, &AAriaCharacter::LookUp);
        EnhancedInputComponent->BindAction(IA_JumpAction, ETriggerEvent::Started, this, &AAriaCharacter::Jump);  // Bind jump on press
        EnhancedInputComponent->BindAction(IA_PickupItem, ETriggerEvent::Started, this, &AAriaCharacter::PickupItem);
    }
}