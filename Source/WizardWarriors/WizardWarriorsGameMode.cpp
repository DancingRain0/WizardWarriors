// Copyright Epic Games, Inc. All Rights Reserved.

#include "WizardWarriorsGameMode.h"

#include "Kismet/GameplayStatics.h"

AWizardWarriorsGameMode::AWizardWarriorsGameMode()
{
}

void AWizardWarriorsGameMode::BeginPlay()
{
    Super::BeginPlay();

    TArray<AActor*> TargetActors;

    UGameplayStatics::GetAllActorsWithTag(
        GetWorld(),
        FName(TEXT("TargetDummy")),
        TargetActors
    );

    TargetsRemaining = TargetActors.Num();

    UE_LOG(
        LogTemp,
        Warning,
        TEXT("Targets remaining: %d"),
        TargetsRemaining
    );

    if (TargetsRemaining <= 0)
    {
        OnAllTargetsDestroyed();
    }
}

int32 AWizardWarriorsGameMode::GetTargetsRemaining() const
{
    return TargetsRemaining;
}

void AWizardWarriorsGameMode::NotifyTargetDestroyed()
{
    TargetsRemaining = FMath::Max(
        TargetsRemaining - 1,
        0
    );

    UE_LOG(
        LogTemp,
        Warning,
        TEXT("Targets remaining: %d"),
        TargetsRemaining
    );

    if (TargetsRemaining == 0)
    {
        OnAllTargetsDestroyed();
    }
}