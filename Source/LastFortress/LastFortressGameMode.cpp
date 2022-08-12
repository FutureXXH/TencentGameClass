// Copyright Epic Games, Inc. All Rights Reserved.

#include "LastFortressGameMode.h"
#include "LastFortressCharacter.h"
#include "UObject/ConstructorHelpers.h"

ALastFortressGameMode::ALastFortressGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
