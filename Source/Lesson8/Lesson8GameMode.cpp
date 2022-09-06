// Copyright Epic Games, Inc. All Rights Reserved.

#include "Lesson8GameMode.h"
#include "Lesson8Character.h"
#include "Kismet/GameplayStatics.h"

#include "UObject/ConstructorHelpers.h"


void ALesson8GameMode::GameRunning()
{
	MyGameStateP->Time -= CurDeltaSeconds;
	if(MyGameStateP->Time <= 0)
	{
		MyGameStateP->GameState = End;
	}
}

void ALesson8GameMode::GameEnd()
{
	for (size_t i = 0; i < PlayerControllers.Num(); i++)
	{
	 
	  if (!IsValid(PlayerControllers[i]))continue;
	   Cast<AMyPlayerController>(PlayerControllers[i])->ShowGameOverUI();
	}
}

ALesson8GameMode::ALesson8GameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/MyCharacter"));
	//static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("bad default Character"));
	}
	PlayerStateClass = AMyPlayerState::StaticClass();
	GameStateClass = AMyGameStateBase::StaticClass();
	PlayerControllerClass = AMyPlayerController::StaticClass();

	PrimaryActorTick.bCanEverTick = true;
	
	
}

void ALesson8GameMode::BeginPlay()
{
	Super::BeginPlay();

	MyGameStateP = GetGameState<AMyGameStateBase>();
}

void ALesson8GameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	if (!IsValid(MyGameStateP))return;

	CurDeltaSeconds = DeltaSeconds;
	
	switch (MyGameStateP->GameState)
	{
	case  Running:
		{
		GameRunning();
		break;
		}
	case End:
		{
		GameEnd();
		MyGameStateP->GameState = Over;
		break;
		}
	}

}

void ALesson8GameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	Cast<AMyPlayerController>(NewPlayer)->ShowHUD();
	PlayerControllers.Add(NewPlayer);
}

void ALesson8GameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
	auto ExitPlayerController = Cast<APlayerController>(Exiting);
	if (!IsValid(ExitPlayerController))return;

	PlayerControllers.Remove(ExitPlayerController);
}

