// Copyright © 2025 Kdean Games. All Rights Reserved


#include "Magic/Spells/BirdsView.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"


// --- Constructor ---
ABirdsView::ABirdsView()
{
    // Enable ticking for this pawn
    PrimaryActorTick.bCanEverTick = true;

    // Create and initialize the capsule component for collision
    BirdsViewCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("BirdsViewCapsule"));
    BirdsViewCapsule->SetCapsuleHalfHeight(24.0f);  // Set half height for bird-like size
    BirdsViewCapsule->SetCapsuleRadius(18.0f);      // Set radius for bird-like size
    // Set BirdsViewCapsule as the root component, replacing the default scene root
    RootComponent = BirdsViewCapsule;

    // Create and initialize the skeletal mesh component for the bird
    BirdsViewMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BirdsViewMesh"));
    BirdsViewMesh->SetupAttachment(GetRootComponent());  // Attach to the capsule root

    // Create and initialize the floating pawn movement component
    BirdsViewMovement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("BirdsViewMovement"));
    BirdsViewMovement->MaxSpeed = 1200.0f; // Default speed
    BirdsViewMovement->Acceleration = 4000.0f; // adjudted for smooth start
    BirdsViewMovement->Deceleration = 8000.0f; // Adjusted for quick stop

    // Create and initialize the spring arm component for camera offset
    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(GetRootComponent());  // Attach to the capsule root
    CameraBoom->TargetArmLength = 400.0f;  // Increased zoom for wider view
    CameraBoom->SetRelativeRotation(FRotator(-15.0f, 0.0f, 0.0f));  // Upward tilt
    CameraBoom->SetRelativeLocation(FVector(0.0f, 0.0f, 50.0f));  // Lift above bird

    // Create and initialize the camera component for the viewpoint
    ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ViewCamera"));
    ViewCamera->SetupAttachment(CameraBoom);  // Attach to the spring arm
    
    // Enable pawn to follow controller rotation for yaw and pitch
    bUseControllerRotationYaw = true;
    bUseControllerRotationPitch = true;

    AutoPossessPlayer = EAutoReceiveInput::Player0;  // Possess with the first controller
}

// --- Begin Play ---
void ABirdsView::BeginPlay()
{
    Super::BeginPlay();
    APlayerController* BirdsViewPlayerController = Cast<APlayerController>(GetController());

    if (BirdsViewPlayerController)
    {
       UEnhancedInputLocalPlayerSubsystem* BirdsViewSubsystem = 
           ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(BirdsViewPlayerController->GetLocalPlayer());

       if (BirdsViewSubsystem)
       {
           BirdsViewSubsystem->AddMappingContext(IMC_BirdsViewMapping, 0);

       }
    }
}

void ABirdsView::BirdMove(const FInputActionValue& Value)
{
    const float DirectionValue = Value.Get<float>();
    if (Controller && (DirectionValue != 0.0f))
    {
        FVector Forward = GetActorForwardVector();
        AddMovementInput(Forward, DirectionValue); // Move forward with W, backward with S
    }
}

// --- Turn (Yaw) ---
void ABirdsView::Turn(const FInputActionValue& Value)
{
    const float YawValue = Value.Get<FVector2D>().X;
    if (Controller)
    {
        AddControllerYawInput(YawValue);  // Rotate controller yaw based on MouseX
    }
}

// --- Look Up (Pitch) ---
void ABirdsView::LookUp(const FInputActionValue& Value)
{
    const float PitchValue = Value.Get<FVector2D>().Y;
    if (Controller)
    {
        AddControllerPitchInput(PitchValue);  // Adjust controller pitch based on MouseY
    }
}

// --- Tick ---
void ABirdsView::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

// --- Setup Player Input ---
void ABirdsView::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    if (UEnhancedInputComponent* EnhancedInputComponent =
        CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
    {
        // Bind movement action
        EnhancedInputComponent->BindAction(
            BirdMoveAction, ETriggerEvent::Triggered, this, &ABirdsView::BirdMove);

        // Bind turn action
        EnhancedInputComponent->BindAction(
            TurnAction, ETriggerEvent::Triggered, this, &ABirdsView::Turn);

        // Bind look up action
        EnhancedInputComponent->BindAction(
            LookUpAction, ETriggerEvent::Triggered, this, &ABirdsView::LookUp);

        // Add mapping context
        if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
        {
            if (UEnhancedInputLocalPlayerSubsystem* Subsystem = 
                ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>
                (PlayerController->GetLocalPlayer()))
            {
                if (IMC_BirdsViewMapping)
                {
                    Subsystem->AddMappingContext(IMC_BirdsViewMapping, 0);
                }
            }
        }
    }
}