// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"


#include "WizardWarriorsCharacter.generated.h"


class UUserWidget;
class USpringArmComponent;
class UCameraComponent;
class UInputAction;

class AArcaneBolt;
class USoundBase;

struct FInputActionValue;
class UAnimMontage;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

/**
 *  A simple player-controllable third person character
 *  Implements a controllable orbiting camera
 */
UCLASS(abstract)
class AWizardWarriorsCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
protected:

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* LookAction;

	/** Mouse Look Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* MouseLookAction;

	/** Primary spell input */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* CastPrimaryAction;

public:

	/** Constructor */
	AWizardWarriorsCharacter();	
	UPROPERTY(
		EditAnywhere,
		BlueprintReadWrite,
		Category = "UI"
	)
	TSubclassOf<UUserWidget> GameHUDClass;

protected:

	/** Initialize input action bindings */
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void BeginPlay() override;

protected:

	void Move(const FInputActionValue& Value);

	void Look(const FInputActionValue& Value);

	void CastPrimary();

public:

	/** Handles move inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoMove(float Right, float Forward);

	/** Handles look inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoLook(float Yaw, float Pitch);

	/** Handles jump pressed inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoJumpStart();

	/** Handles jump pressed inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoJumpEnd();

	void SpawnPrimaryProjectile();

	UPROPERTY(
		EditAnywhere,
		BlueprintReadWrite,
		Category = "Spells"
	)
	TObjectPtr<UAnimMontage> PrimaryCastMontage = nullptr;

	UPROPERTY(
		EditAnywhere,
		BlueprintReadWrite,
		Category = "Spells"
	)
	float PrimarySpellManaCost = 20.0f;

	UPROPERTY(
		EditAnywhere,
		BlueprintReadWrite,
		Category = "Spells"
	)
	TObjectPtr<USoundBase> PrimaryCastSound = nullptr;
public:

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	/** Arcane Bolt Blueprint spawned by the primary spell */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spells", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AArcaneBolt> ArcaneBoltClass;

};

