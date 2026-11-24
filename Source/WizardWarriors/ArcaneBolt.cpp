// Copyright Epic Games, Inc. All Rights Reserved.

#include "ArcaneBolt.h"
#include "Kismet/GameplayStatics.h"
#include "HealthComponent.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Engine/Engine.h"

AArcaneBolt::AArcaneBolt()
{
    PrimaryActorTick.bCanEverTick = false;

    CollisionSphere =
        CreateDefaultSubobject<USphereComponent>(
            TEXT("CollisionSphere")
        );

    CollisionSphere->InitSphereRadius(15.0f);

    CollisionSphere->SetCollisionProfileName(
        TEXT("BlockAllDynamic")
    );

    CollisionSphere->SetCollisionEnabled(
        ECollisionEnabled::QueryAndPhysics
    );

    CollisionSphere->SetNotifyRigidBodyCollision(true);

    CollisionSphere->OnComponentHit.AddDynamic(
        this,
        &AArcaneBolt::OnHit
    );

    RootComponent = CollisionSphere;

    ProjectileMovement =
        CreateDefaultSubobject<UProjectileMovementComponent>(
            TEXT("ProjectileMovement")
        );

    ProjectileMovement->SetUpdatedComponent(CollisionSphere);
    ProjectileMovement->InitialSpeed = 1500.0f;
    ProjectileMovement->MaxSpeed = 1500.0f;
    ProjectileMovement->bRotationFollowsVelocity = true;
    ProjectileMovement->bShouldBounce = false;
    ProjectileMovement->ProjectileGravityScale = 0.0f;

    InitialLifeSpan = 3.0f;
}

void AArcaneBolt::BeginPlay()
{
    Super::BeginPlay();

    AActor* OwnerActor = GetOwner();

    if (OwnerActor)
    {
        CollisionSphere->IgnoreActorWhenMoving(
            OwnerActor,
            true
        );
    }
}

void AArcaneBolt::OnHit(
    UPrimitiveComponent* HitComponent,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComponent,
    FVector NormalImpulse,
    const FHitResult& Hit
)
{
    if (
        OtherActor &&
        OtherActor != this &&
        OtherActor != GetOwner()
        )
    {
        if (
            UHealthComponent* HealthComponent =
            OtherActor->FindComponentByClass<UHealthComponent>()
            )
        {
            HealthComponent->ApplyDamage(DamageAmount);
        }

        if (ImpactEffect)
        {
            UNiagaraFunctionLibrary::SpawnSystemAtLocation(
                GetWorld(),
                ImpactEffect,
                Hit.ImpactPoint,
                Hit.ImpactNormal.Rotation()
            );
        }
        if (ImpactSound)
        {
            UGameplayStatics::PlaySoundAtLocation(
                this,
                ImpactSound,
                Hit.ImpactPoint
            );
        }
        Destroy();
    }
}