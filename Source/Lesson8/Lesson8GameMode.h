// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/GameSession.h"
#include "Lesson8/Public/MyPlayerState.h"
#include "Lesson8/Public/MyGameStateBase.h"
#include "Lesson8/Public/MyPlayerController.h"
#include "Lesson8GameMode.generated.h"

UCLASS(minimalapi)
class  ALesson8GameMode : public AGameModeBase
{
	GENERATED_BODY()

	UFUNCTION()
		void GameRunning();
	UFUNCTION()
		void GameEnd();

	TArray<APlayerController*> PlayerControllers;
public:
	ALesson8GameMode();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	AMyGameStateBase* MyGameStateP;

	float CurDeltaSeconds;

	virtual void PostLogin(APlayerController* NewPlayer) override;

	virtual void Logout(AController* Exiting) override;

};



