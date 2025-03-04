// Fill out your copyright notice in the Description page of Project Settings.


#include "SwordTraceNotifyState.h"

#include "TestRPG/Characters/MainCharacter.h"


void USwordTraceNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (AMainCharacter* MainCharacter = Cast<AMainCharacter>(MeshComp->GetOwner()))
	{
		MainCharacter->GetCombatComponent()->SwordTrace_Start();
	}
}

void USwordTraceNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (AMainCharacter* MainCharacter = Cast<AMainCharacter>(MeshComp->GetOwner()))
	{
		MainCharacter->GetCombatComponent()->SwordTrace_End();
	}
}
