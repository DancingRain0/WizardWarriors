#include "ManaComponent.h"

UManaComponent::UManaComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UManaComponent::BeginPlay()
{
    Super::BeginPlay();

    CurrentMana = MaxMana;
}

void UManaComponent::TickComponent(
    float DeltaTime,
    ELevelTick TickType,
    FActorComponentTickFunction* ThisTickFunction
)
{
    Super::TickComponent(
        DeltaTime,
        TickType,
        ThisTickFunction
    );

    CurrentMana = FMath::Clamp(
        CurrentMana +
        ManaRegenerationPerSecond * DeltaTime,
        0.0f,
        MaxMana
    );
}

bool UManaComponent::TrySpendMana(float ManaCost)
{
    if (ManaCost <= 0.0f)
    {
        return true;
    }

    if (CurrentMana < ManaCost)
    {
        return false;
    }

    CurrentMana -= ManaCost;

    return true;
}

float UManaComponent::GetCurrentMana() const
{
    return CurrentMana;
}

float UManaComponent::GetMaxMana() const
{
    return MaxMana;
}

float UManaComponent::GetManaPercent() const
{
    if (MaxMana <= 0.0f)
    {
        return 0.0f;
    }

    return CurrentMana / MaxMana;
}