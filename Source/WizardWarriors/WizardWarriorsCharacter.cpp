// Copyright Epic Games, Inc. All Rights Reserved.

#include "WizardWarriorsCharacter.h"

#include "Animation/AnimMontage.h"
#include "Animation/AnimInstance.h"

#include "ArcaneBolt.h"
#include "Kismet/GameplayStatics.h"
#include "ManaComponent.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputComponent.h"

#include "Engine/World.h"
#include "Blueprint/UserWidget.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputActionValue.h"

#include "WizardWarriors.h"

AWizardWarriorsCharacter::AWizardWarriorsCharacter()
{
    // Configure the character's collision capsule.
    GetCapsuleComponent()->InitCapsuleSize(42.0f, 96.0f);

    // The character follows the controller's horizontal rotation.
    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = true;
    bUseControllerRotationRoll = false;

    // The character faces the camera/controller direction rather
    // than automatically turning toward movement.
    GetCharacterMovement()->bOrientRotationToMovement = false;

    GetCharacterMovement()->RotationRate =
        FRotator(0.0f, 500.0f, 0.0f);

    // Movement settings.
    GetCharacterMovement()->JumpZVelocity = 500.0f;
    GetCharacterMovement()->AirControl = 0.35f;
    GetCharacterMovement()->MaxWalkSpeed = 500.0f;
    GetCharacterMovement()->MinAnalogWalkSpeed = 20.0f;
    GetCharacterMovement()->BrakingDecelerationWalking = 2000.0f;
    GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

    // Create the camera boom.
    CameraBoom =
        CreateDefaultSubobject<USpringArmComponent>(
            TEXT("CameraBoom")
        );

    CameraBoom->SetupAttachment(RootComponent);
    CameraBoom->TargetArmLength = 400.0f;
    CameraBoom->bUsePawnControlRotation = true;

    // Create the follow camera.
    FollowCamera =
        CreateDefaultSubobject<UCameraComponent>(
            TEXT("FollowCamera")
        );

    FollowCamera->SetupAttachment(
        CameraBoom,
        USpringArmComponent::SocketName
    );

    FollowCamera->bUsePawnControlRotation = false;
}

void AWizardWarriorsCharacter::SetupPlayerInputComponent(
    UInputComponent* PlayerInputComponent
)
{
    if (
        UEnhancedInputComponent* EnhancedInputComponent =
        Cast<UEnhancedInputComponent>(PlayerInputComponent)
        )
    {
        // Jump input.
        EnhancedInputComponent->BindAction(
            JumpAction,
            ETriggerEvent::Started,
            this,
            &ACharacter::Jump
        );

        EnhancedInputComponent->BindAction(
            JumpAction,
            ETriggerEvent::Completed,
            this,
            &ACharacter::StopJumping
        );

        // Movement input.
        EnhancedInputComponent->BindAction(
            MoveAction,
            ETriggerEvent::Triggered,
            this,
            &AWizardWarriorsCharacter::Move
        );

        // Mouse camera input.
        EnhancedInputComponent->BindAction(
            MouseLookAction,
            ETriggerEvent::Triggered,
            this,
            &AWizardWarriorsCharacter::Look
        );

        // Controller camera input.
        EnhancedInputComponent->BindAction(
            LookAction,
            ETriggerEvent::Triggered,
            this,
            &AWizardWarriorsCharacter::Look
        );

        // Primary spell input.
        EnhancedInputComponent->BindAction(
            CastPrimaryAction,
            ETriggerEvent::Started,
            this,
            &AWizardWarriorsCharacter::CastPrimary
        );
    }
    else
    {
        UE_LOG(
            LogWizardWarriors,
            Error,
            TEXT(
                "'%s' failed to find an Enhanced Input component!"
            ),
            *GetNameSafe(this)
        );
    }
}

void AWizardWarriorsCharacter::BeginPlay()
{
    Super::BeginPlay();

    if (
        IsLocallyControlled() &&
        GameHUDClass
        )
    {
        UUserWidget* GameHUD =
            CreateWidget<UUserWidget>(
                GetWorld(),
                GameHUDClass
            );

        if (GameHUD)
        {
            GameHUD->AddToViewport();
        }
    }
}

void AWizardWarriorsCharacter::Move(
    const FInputActionValue& Value
)
{
    const FVector2D MovementVector =
        Value.Get<FVector2D>();

    DoMove(
        MovementVector.X,
        MovementVector.Y
    );
}

void AWizardWarriorsCharacter::Look(
    const FInputActionValue& Value
)
{
    const FVector2D LookAxisVector =
        Value.Get<FVector2D>();

    DoLook(
        LookAxisVector.X,
        LookAxisVector.Y
    );
}

void AWizardWarriorsCharacter::DoMove(
    float Right,
    float Forward
)
{
    if (!GetController())
    {
        return;
    }

    // Only use the controller's horizontal rotation.
    const FRotator ControlRotation =
        GetController()->GetControlRotation();

    const FRotator YawRotation(
        0.0f,
        ControlRotation.Yaw,
        0.0f
    );

    // Determine camera-relative forward and right directions.
    const FVector ForwardDirection =
        FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

    const FVector RightDirection =
        FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

    AddMovementInput(ForwardDirection, Forward);
    AddMovementInput(RightDirection, Right);
}

void AWizardWarriorsCharacter::DoLook(
    float Yaw,
    float Pitch
)
{
    if (!GetController())
    {
        return;
    }

    AddControllerYawInput(Yaw);
    AddControllerPitchInput(Pitch);
}

void AWizardWarriorsCharacter::DoJumpStart()
{
    Jump();
}

void AWizardWarriorsCharacter::DoJumpEnd()
{
    StopJumping();
}

void AWizardWarriorsCharacter::CastPrimary()
{
    if (!PrimaryCastMontage)
    {
        SpawnPrimaryProjectile();
        return;
    }


    UAnimInstance* AnimInstance =
        GetMesh()->GetAnimInstance();
    if (
        AnimInstance &&
        AnimInstance->Montage_IsPlaying(PrimaryCastMontage)
        )
    {
        return;
    }

    UManaComponent* ManaComponent =
        FindComponentByClass<UManaComponent>();

    if (!ManaComponent)
    {
        UE_LOG(
            LogTemp,
            Error,
            TEXT("Player does not have a ManaComponent!")
        );

        return;
    }

    if (
        !ManaComponent->TrySpendMana(
            PrimarySpellManaCost
        )
        )
    {
        UE_LOG(
            LogTemp,
            Warning,
            TEXT("Not enough mana to cast!")
        );

        return;
    }

    if (PrimaryCastSound)
    {
        UGameplayStatics::PlaySoundAtLocation(
            this,
            PrimaryCastSound,
            GetActorLocation()
        );
    }
    PlayAnimMontage(PrimaryCastMontage);
}

void AWizardWarriorsCharacter::SpawnPrimaryProjectile()
{
    // The projectile cannot be spawned without a class or world.
    if (!ArcaneBoltClass || !GetWorld())
    {
        return;
    }

    // Get the socket's current animated world position.
    const FName SpawnBoneName = TEXT("hand_r");

    if (GetMesh()->GetBoneIndex(SpawnBoneName) == INDEX_NONE)
    {
        UE_LOG(
            LogTemp,
            Error,
            TEXT("Could not find bone: %s"),
            *SpawnBoneName.ToString()
        );

        return;
    }

    const FVector HandLocation =
        GetMesh()->GetBoneLocation(SpawnBoneName);

    const FVector SpawnLocation =
        HandLocation + GetControlRotation().Vector() * 10.0f;

    // Fire in the direction the camera/controller faces.
    const FRotator SpawnRotation =
        GetControlRotation();

    FActorSpawnParameters SpawnParameters;

    // Record which character created the projectile.
    SpawnParameters.Owner = this;
    SpawnParameters.Instigator = this;

    // Permit spawning even if the socket briefly overlaps something.
    SpawnParameters.SpawnCollisionHandlingOverride =
        ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    AArcaneBolt* SpawnedBolt =
        GetWorld()->SpawnActor<AArcaneBolt>(
            ArcaneBoltClass,
            SpawnLocation,
            SpawnRotation,
            SpawnParameters
        );

    if (SpawnedBolt)
    {
        // Prevent the player's capsule from blocking its own bolt.
        GetCapsuleComponent()->IgnoreActorWhenMoving(
            SpawnedBolt,
            true
        );
    }
}