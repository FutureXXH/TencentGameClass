// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class LESSON8_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()
public:

	AMyPlayerController();

	virtual void BeginPlay() override;


	UFUNCTION(Client, Reliable)
		void ShowHUD();
	  void ShowHUD_Implementation();

	UFUNCTION(Client,Reliable)
		void ShowGameOverUI();
	    void ShowGameOverUI_Implementation();

	UPROPERTY()
		UUserWidget* HUD;
	    UUserWidget* GameEndUI;
	    
	
};
