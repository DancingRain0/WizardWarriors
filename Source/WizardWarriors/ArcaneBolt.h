// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ArcaneBolt.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class USoundBase;
class UPrimitiveComponent;
class UNiagaraSystem;

UCLASS()
class WIZARDWARRIORS_API AArcaneBolt : public AActor
{
    GENERATED_BODY()

public:
    AArcaneBolt();

    UPROPERTY(
        EditAnywhere,
        BlueprintReadWrite,
        Category = "Effects"
    )
    TObjectPtr<UNiagaraSystem> ImpactEffect = nullptr;

    UPROPERTY(
        EditAnywhere,
        BlueprintReadWrite,
        Category = "Effects"
    )
    TObjectPtr<USoundBase> ImpactSound = nullptr;

    UPROPERTY(
        EditAnywhere,
        BlueprintReadWrite,
        Category = "Damage"
    )
    float DamageAmount = 25.0f;



protected:
    virtual void BeginPlay() override;

private:
    UFUNCTION()
    void OnHit(
        UPrimitiveComponent* HitComponent,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComponent,
        FVector NormalImpulse,
        const FHitResult& Hit
    );

    UPROPERTY(
        VisibleAnywhere,
        BlueprintReadOnly,
        Category = "Projectile",
        meta = (AllowPrivateAccess = "true")
    )
    TObjectPtr<USphereComponent> CollisionSphere;

    UPROPERTY(
        VisibleAnywhere,
        BlueprintReadOnly,
        Category = "Projectile",
        meta = (AllowPrivateAccess = "true")
    )
    TObjectPtr<UProjectileMovementComponent> ProjectileMovement;
};