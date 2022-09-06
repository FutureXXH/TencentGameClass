// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "Net/UnrealNetwork.h"
#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "MyGameStateBase.generated.h"

/**
 * 
 */

UENUM()
enum EGameState
{

	Running = 1,
	End = 2,
	Over = 3

};

UCLASS()
class LESSON8_API AMyGameStateBase : public AGameStateBase
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Replicated)
	float Time = 60;

	EGameState GameState = Running;

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
