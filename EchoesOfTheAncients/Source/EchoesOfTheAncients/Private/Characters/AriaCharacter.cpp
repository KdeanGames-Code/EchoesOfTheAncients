// Copyright © 2025 Kdean Games. All Rights Reserved
// Implements the AAriaCharacter class, handling Aria’s movement, camera, 
// and interactions in Echoes of the Ancients.

#include "Characters/AriaCharacter.h"
#include "Animation/AnimMontage.h"
#include "Camera/CameraComponent.h"
#include "Characters/CharacterTypes.h"
#include "Components/BoxComponent.h"
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
    CameraBoom->bUsePawnControlRotation = true;

    // Create and initialize the groom component for hair
    Hair = CreateDefaultSubobject<UGroomComponent>(TEXT("Hair"));
    Hair->SetupAttachment(GetMesh());
    Hair->AttachmentName = FString("head");  // Attach to head socket

    // Create and initialize the groom component for eyebrows
    Eyebrows = CreateDefaultSubobject<UGroomComponent>(TEXT("Eyebrows"));
    Eyebrows->SetupAttachment(GetMesh());
    Eyebrows->AttachmentName = FString("head");  // Attach to head socket

    TotalDawnspiresCollected = 0;
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

// --- SetWeaponCollisionEnabled ---
// Sets the collision enabled state for the equipped weapon’s WeaponBox.
void AAriaCharacter::SetWeaponCollisionEnabled(ECollisionEnabled::Type CollisionEnabled)
{
    // Check if a weapon is equipped and its WeaponBox exists before setting collision.
    if (EquippedWeapon && EquippedWeapon->GetWeaponBox())
    {
        EquippedWeapon->GetWeaponBox()->SetCollisionEnabled(CollisionEnabled);
        // Clear the IgnoreActors array when disabling collision to reset for the next attack.
        EquippedWeapon->IgnoreActors.Empty();
    }
}

// --- Move ---
// Moves Aria forward or backward based on the input action value (W/S keys).
void AAriaCharacter::Move(const FInputActionValue& Value)
{
    // Prevent movement if Aria is attacking to avoid foot sliding.
    if (ActionState != EActionState::EAS_Unoccupied) return;

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
    // Prevent movement if Aria is attacking to avoid foot sliding.
    if (ActionState != EActionState::EAS_Unoccupied) return;

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
        // Equip the weapon if overlapping one.
        OverlappingWeapon->Equip(GetMesh(), FName("R_hand_weapon"), this, this);  // Equip the weapon to Aria’s right hand socket.
        OverlappingItem = nullptr;  // Clear the overlapping item to prevent re-equipping the same weapon.
        EquippedWeapon = OverlappingWeapon;  // Store the equipped weapon.
        CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;  // Update Aria’s state to reflect she’s equipped a one-handed weapon.
    }
    else
    {
        // If not overlapping a weapon, check if we can disarm (unequip) the current weapon.
        if (CanDisarm())
        {
            PlayEquipMontage(FName("UnEquip"));
            CharacterState = ECharacterState::ECS_Unequipped;  // Set state to unequipped.
            ActionState = EActionState::EAS_EquippingWeapon;  // Set state to equipping to prevent movement.
        }
        // If we can’t disarm, check if we can arm (equip) the weapon from the back.
        else if (CanArm())
        {
            PlayEquipMontage(FName("Equip"));
            CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;  // Set state to equipped.
            ActionState = EActionState::EAS_EquippingWeapon;  // Set state to equipping to prevent movement.
        }
    }
}

/// Increments the Dawnspire count when a Dawnspire coin is collected, 
/// tracking progress toward unlocking the Sanctum of Echoes.
void AAriaCharacter::IncrementDawnspireCount()
{
    TotalDawnspiresCollected++;
    // In future, we'll check if TotalDawnspiresCollected >= 100 to unlock the Sanctum of Echoes
}

// --- Attack ---
// Callback for the left mouse button press to play a random section of the attack montage.
void AAriaCharacter::Attack_OneHanded()
{
    // Only allow attacking if the character can attack (unoccupied and equipped).
    if (CanAttack_OneHandedWeapon())
    {
        PlayAttackMontage_OneHandedWeapon();
        // Set the action state to attacking to prevent spamming.
        ActionState = EActionState::EAS_Attacking;
    }
}

// --- CanAttack_OneHandedWeapon ---
// Checks if Aria can perform a one-handed attack based on her current state.
bool AAriaCharacter::CanAttack_OneHandedWeapon()
{
    return ActionState == EActionState::EAS_Unoccupied &&
        CharacterState != ECharacterState::ECS_Unequipped;
}

// --- PlayAttackMontage_OneHandedWeapon ---
// Plays the one-handed attack montage with a random section.
void AAriaCharacter::PlayAttackMontage_OneHandedWeapon()
{
    // Get the animation instance from the character’s mesh.
    UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

    // Check if the anim instance and attack montage are valid before proceeding.
    if (AnimInstance && Attack_OneHandedWeapon)
    {
        // Play the attack montage with default settings (play rate 1.0, start at 0.0, stop all other montages).
        AnimInstance->Montage_Play(Attack_OneHandedWeapon);

        // Randomly select between Attack1, Attack2, and Attack3 sections (0, 1, or 2).
        const int32 Selection = FMath::RandRange(0, 2);
        FName SectionName = FName();

        // Set the section name based on the random selection.
        switch (Selection)
        {
        case 0:
            SectionName = FName("Attack1");
            break;
        case 1:
            SectionName = FName("Attack2");
            break;
        case 2:
            SectionName = FName("Attack3");
            break;
        default:
            break;
        }

        // Jump to the selected section in the attack montage.
        AnimInstance->Montage_JumpToSection(SectionName, Attack_OneHandedWeapon);
    }
}

// --- CanArm ---
// Checks if Aria can arm (equip) her weapon based on her current state.
bool AAriaCharacter::CanArm()
{
    return ActionState == EActionState::EAS_Unoccupied &&
        CharacterState == ECharacterState::ECS_Unequipped &&
        EquippedWeapon != nullptr;
}

// --- CanDisarm ---
// Checks if Aria can disarm (unequip) her weapon based on her current state.
bool AAriaCharacter::CanDisarm()
{
    return ActionState == EActionState::EAS_Unoccupied &&
        CharacterState != ECharacterState::ECS_Unequipped &&
        EquippedWeapon != nullptr;
}

// --- Disarm ---
// Attaches the equipped weapon to the spine socket (unequip).
void AAriaCharacter::Disarm()
{
    if (EquippedWeapon)
    {
        EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("SpineSocket"));
    }
}

// --- Arm ---
// Attaches the equipped weapon to the right hand socket (equip).
void AAriaCharacter::Arm()
{
    if (EquippedWeapon)
    {
        EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("R_hand_weapon"));
    }
}

// --- FinishEquipping ---
// Resets the action state to unoccupied after an equip/unequip montage finishes.
void AAriaCharacter::FinishEquipping()
{
    ActionState = EActionState::EAS_Unoccupied;
}

// --- PlayEquipMontage ---
// Plays the equip/unequip montage with the specified section.
void AAriaCharacter::PlayEquipMontage(const FName& SectionName)
{
    // Get the animation instance from the character’s mesh.
    UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
    // Check if the anim instance and equip montage are valid before proceeding.
    if (AnimInstance && EquipMontage)
    {
        // Play the equip montage with default settings.
        AnimInstance->Montage_Play(EquipMontage);
        // Jump to the specified section in the equip montage.
        AnimInstance->Montage_JumpToSection(SectionName, EquipMontage);
    }
}

// --- AttackEnd ---
// Resets the action state to unoccupied after an attack montage finishes.
void AAriaCharacter::AttackEnd()
{
    ActionState = EActionState::EAS_Unoccupied;
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
        EnhancedInputComponent->BindAction(IA_JumpAction, ETriggerEvent::Triggered, this, &AAriaCharacter::Jump);  // Bind jump on press
        EnhancedInputComponent->BindAction(IA_PickupItem, ETriggerEvent::Triggered, this, &AAriaCharacter::PickupItem);
        EnhancedInputComponent->BindAction(IA_Attack_OneHanded, ETriggerEvent::Started, this, &AAriaCharacter::Attack_OneHanded);  // Bind attack action on press (Left Mouse Button).
    }
}