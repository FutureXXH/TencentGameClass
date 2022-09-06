// Fill out your copyright notice in the Description page of Project Settings.


#include "Lesson8/Public/MyPlayerController.h"


TSubclassOf<UUserWidget> HUDClass;
TSubclassOf<UUserWidget> GameEndUIClass;

AMyPlayerController::AMyPlayerController()
{
	static ConstructorHelpers::FClassFinder<UUserWidget> BP_HUD(TEXT("WidgetBlueprint'/Game/Blueprints/UI/HUD.HUD_C'"));
	static ConstructorHelpers::FClassFinder<UUserWidget> BP_GameEnd(TEXT("WidgetBlueprint'/Game/Blueprints/UI/EndGame.EndGame_C'"));

	if (BP_HUD.Succeeded())
	{
		HUDClass =  BP_HUD.Class;
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("bad Create HUD"));
	}
	if(BP_GameEnd.Succeeded())
	{
		GameEndUIClass = BP_GameEnd.Class;
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("bad Create GameEndUI"));
	}
}

void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();


}

void AMyPlayerController::ShowHUD_Implementation()
{

	HUD = CreateWidget<UUserWidget>(GetWorld(), HUDClass);
	HUD->AddToViewport();
}

void AMyPlayerController::ShowGameOverUI_Implementation()
{
	HUD->RemoveFromParent();
	GameEndUI = CreateWidget<UUserWidget>(GetWorld(), GameEndUIClass);
	GameEndUI->AddToViewport();

	 GetPawn()->DisableInput(this);
}

