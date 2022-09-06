// Fill out your copyright notice in the Description page of Project Settings.


#include "Lesson8/Public/MyPlayerState.h"



void AMyPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//复制当前生命值。
	DOREPLIFETIME(AMyPlayerState, KillCount);
	DOREPLIFETIME(AMyPlayerState, DeathCount);
}
