// Fill out your copyright notice in the Description page of Project Settings.
#include "WizardWarriorsGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "HealthComponent.h"

UHealthComponent::UHealthComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UHealthComponent::BeginPlay()
{
    Super::BeginPlay();

    CurrentHealth = MaxHealth;
}
void UHealthComponent::ApplyDamage(float DamageAmount)
{
    if (DamageAmount <= 0.0f)
    {
        return;
    }

    CurrentHealth = FMath::Clamp(
        CurrentHealth - DamageAmount,
        0.0f,
        MaxHealth
    );

    UE_LOG(
        LogTemp,
        Warning,
        TEXT("%s has %.1f health remaining"),
        *GetOwner()->GetName(),
        CurrentHealth
    );

    if (CurrentHealth <= 0.0f)
    {
        AActor* OwnerActor = GetOwner();

        if (!OwnerActor)
        {
            return;
        }
        if (DeathSound)
        {
            UGameplayStatics::PlaySoundAtLocation(
                this,
                DeathSound,
                OwnerActor->GetActorLocation()
            );
        }
        if (OwnerActor->ActorHasTag(TEXT("TargetDummy")))
        {
            AWizardWarriorsGameMode* GameMode =
                Cast<AWizardWarriorsGameMode>(
                    GetWorld()->GetAuthGameMode()
                );

            if (GameMode)
            {
                GameMode->NotifyTargetDestroyed();
            }
        }

        OwnerActor->Destroy();
    }


}

float UHealthComponent::GetCurrentHealth() const
{
    return CurrentHealth;
}

float UHealthComponent::GetMaxHealth() const
{
    return MaxHealth;
}

float UHealthComponent::GetHealthPercent() const
{
    if (MaxHealth <= 0.0f)
    {
        return 0.0f;
    }

    return CurrentHealth / MaxHealth;
}