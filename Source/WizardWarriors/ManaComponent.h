#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ManaComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class WIZARDWARRIORS_API UManaComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UManaComponent();

    virtual void TickComponent(
        float DeltaTime,
        ELevelTick TickType,
        FActorComponentTickFunction* ThisTickFunction
    ) override;

    UFUNCTION(BlueprintCallable, Category = "Mana")
    bool TrySpendMana(float ManaCost);

    UFUNCTION(BlueprintPure, Category = "Mana")
    float GetCurrentMana() const;

    UFUNCTION(BlueprintPure, Category = "Mana")
    float GetMaxMana() const;

    UFUNCTION(BlueprintPure, Category = "Mana")
    float GetManaPercent() const;

protected:
    virtual void BeginPlay() override;

private:
    UPROPERTY(
        EditAnywhere,
        BlueprintReadOnly,
        Category = "Mana",
        meta = (AllowPrivateAccess = "true")
    )
    float MaxMana = 100.0f;

    UPROPERTY(
        VisibleAnywhere,
        BlueprintReadOnly,
        Category = "Mana",
        meta = (AllowPrivateAccess = "true")
    )
    float CurrentMana = 0.0f;

    UPROPERTY(
        EditAnywhere,
        BlueprintReadOnly,
        Category = "Mana",
        meta = (AllowPrivateAccess = "true")
    )
    float ManaRegenerationPerSecond = 10.0f;
};