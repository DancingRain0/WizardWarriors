// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "WizardWarriorsGameMode.generated.h"

UCLASS(abstract)
class WIZARDWARRIORS_API AWizardWarriorsGameMode
    : public AGameModeBase
{
    GENERATED_BODY()

public:
    AWizardWarriorsGameMode();

    UFUNCTION(BlueprintPure, Category = "Targets")
    int32 GetTargetsRemaining() const;

    UFUNCTION(BlueprintCallable, Category = "Targets")
    void NotifyTargetDestroyed();

    UFUNCTION(BlueprintImplementableEvent, Category = "Targets")
    void OnAllTargetsDestroyed();

protected:
    virtual void BeginPlay() override;

private:
    UPROPERTY(
        VisibleAnywhere,
        BlueprintReadOnly,
        Category = "Targets",
        meta = (AllowPrivateAccess = "true")
    )
    int32 TargetsRemaining = 0;
};