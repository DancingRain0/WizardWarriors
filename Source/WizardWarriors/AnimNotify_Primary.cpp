#include "AnimNotify_Primary.h"
#include "TimerManager.h"
#include "Components/SkeletalMeshComponent.h"
#include "WizardWarriorsCharacter.h"

void UAnimNotify_Primary::Notify(
    USkeletalMeshComponent* MeshComp,
    UAnimSequenceBase* Animation,
    const FAnimNotifyEventReference& EventReference
)
{
    Super::Notify(
        MeshComp,
        Animation,
        EventReference
    );

    if (!MeshComp)
    {
        return;
    }

    AWizardWarriorsCharacter* Character =
        Cast<AWizardWarriorsCharacter>(
            MeshComp->GetOwner()
        );

    if (Character)
    {
        Character->GetWorldTimerManager().SetTimerForNextTick(
            FTimerDelegate::CreateUObject(
                Character,
                &AWizardWarriorsCharacter::SpawnPrimaryProjectile
            )
        );
    }
}