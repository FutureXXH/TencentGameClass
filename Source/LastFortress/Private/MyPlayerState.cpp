// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerState.h"

void AMyPlayerState::SetMyPlayerName(const FString& S)
{
	SetPlayerName(S);
	SessionName = FName(S);
	
}
