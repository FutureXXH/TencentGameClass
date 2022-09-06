// Fill out your copyright notice in the Description page of Project Settings.



#include "Lesson8/Public/MyGameStateBase.h"



void AMyGameStateBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	//复制当前生命值。
	DOREPLIFETIME(AMyGameStateBase, Time);
}
