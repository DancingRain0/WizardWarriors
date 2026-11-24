// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

class USoundBase;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class WIZARDWARRIORS_API UHealthComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UHealthComponent();

    UFUNCTION(BlueprintCallable, Category = "Health")
    void ApplyDamage(float DamageAmount);

    UFUNCTION(BlueprintPure, Category = "Health")
    float GetCurrentHealth() const;

    UFUNCTION(BlueprintPure, Category = "Health")
    float GetMaxHealth() const;

    UFUNCTION(BlueprintPure, Category = "Health")
    float GetHealthPercent() const;

protected:
    virtual void BeginPlay() override;

private:
    UPROPERTY(
        EditAnywhere,
        BlueprintReadOnly,
        Category = "Health",
        meta = (AllowPrivateAccess = "true")
    )
    float MaxHealth = 100.0f;

    UPROPERTY(
        VisibleAnywhere,
        BlueprintReadOnly,
        Category = "Health",
        meta = (AllowPrivateAccess = "true")
    )
    float CurrentHealth = 0.0f;

    UPROPERTY(
        EditAnywhere,
        BlueprintReadOnly,
        Category = "Effects",
        meta = (AllowPrivateAccess = "true")
    )
    TObjectPtr<USoundBase> DeathSound = nullptr;
};
