// Fill out your copyright notice in the Description page of Project Settings.



#include "Lesson8/Public/MyGameStateBase.h"



void AMyGameStateBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	//���Ƶ�ǰ����ֵ��
	DOREPLIFETIME(AMyGameStateBase, Time);
}
