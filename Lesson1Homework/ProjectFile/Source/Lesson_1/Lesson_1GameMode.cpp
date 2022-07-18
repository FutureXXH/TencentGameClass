// Copyright Epic Games, Inc. All Rights Reserved.

#include "Lesson_1GameMode.h"
#include "Lesson_1Character.h"
#include "UObject/ConstructorHelpers.h"

ALesson_1GameMode::ALesson_1GameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
