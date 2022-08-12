// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"

#include "MyPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class LASTFORTRESS_API AMyPlayerState : public APlayerState
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = PlayerState)
	void SetMyPlayerName(const FString& S) ;

	
};
