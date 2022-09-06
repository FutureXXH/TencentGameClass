// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/PlayerState.h"
#include "MyPlayerState.generated.h"

/**
 * 
 */



UCLASS()
class LESSON8_API AMyPlayerState : public APlayerState
{
	GENERATED_BODY()
public:
	/**  Ù–‘∏¥÷∆ */
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Replicated)
		int KillCount = 0;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Replicated)
		int DeathCount = 0;

	
};
