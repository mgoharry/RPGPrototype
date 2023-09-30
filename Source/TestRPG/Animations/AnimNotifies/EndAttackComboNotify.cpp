// Fill out your copyright notice in the Description page of Project Settings.


#include "EndAttackComboNotify.h"

#include "TestRPG/Characters/MainCharacter.h"

void UEndAttackComboNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (AMainCharacter* MainCharacter = Cast<AMainCharacter>(MeshComp->GetOwner())) {
		if (MainCharacter->GetCombatComponent()) {
			MainCharacter->GetCombatComponent()->SwordAttackEnd();
		}
	}
}
