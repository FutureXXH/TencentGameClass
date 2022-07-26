// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomCreateSessionCallbackProxy.h"
#include "EngineGlobals.h"
#include "Engine/Engine.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/PlayerState.h"
#include "../Plugins/Online/OnlineSubsystem/Source/Public/OnlineSessionSettings.h"
#include "../Plugins/Online/OnlineSubsystemUtils/Source/OnlineSubsystemUtils/Public/OnlineSubsystemUtils.h"

UCustomCreateSessionCallbackProxy::UCustomCreateSessionCallbackProxy(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, CreateCompleteDelegate(FOnCreateSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnCreateCompleted))
	, StartCompleteDelegate(FOnStartSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnStartCompleted))
	, NumPublicConnections(1)
{
}

UCustomCreateSessionCallbackProxy* UCustomCreateSessionCallbackProxy::CreateCustomSession(UObject* WorldContextObject, class APlayerController* PlayerController, int32 PublicConnections, bool bUseLAN)
{
	UCustomCreateSessionCallbackProxy* Proxy = NewObject<UCustomCreateSessionCallbackProxy>();
	Proxy->PlayerControllerWeakPtr = PlayerController;
	Proxy->NumPublicConnections = PublicConnections;
	Proxy->bUseLAN = bUseLAN;
	Proxy->WorldContextObject = WorldContextObject;
	return Proxy;
}

void UCustomCreateSessionCallbackProxy::Activate()
{
	FCunstomOnlineSubsystemBPCallHelper Helper(TEXT("CreateSession"), WorldContextObject);
	Helper.QueryIDFromPlayerController(PlayerControllerWeakPtr.Get());

	if (Helper.IsValid())
	{
		auto Sessions = Helper.OnlineSub->GetSessionInterface();
		if (Sessions.IsValid())
		{
			CreateCompleteDelegateHandle = Sessions->AddOnCreateSessionCompleteDelegate_Handle(CreateCompleteDelegate);

			FOnlineSessionSettings Settings;
			Settings.NumPublicConnections = NumPublicConnections;
			Settings.bShouldAdvertise = true;
			Settings.bAllowJoinInProgress = true;
			Settings.bIsLANMatch = bUseLAN;
			Settings.bUsesPresence = true;
			Settings.bAllowJoinViaPresence = true;

			// DedicatedServer
			if (!Helper.UserID.IsValid())
			{
				Sessions->CreateSession(666, NAME_GameSession, Settings);
			}
			// Client Host.
			else
			{
				Sessions->CreateSession(*Helper.UserID, NAME_GameSession, Settings);
			}
			
			
			return;
		}
		else
		{
			FFrame::KismetExecutionMessage(TEXT("Sessions not supported by Online Subsystem"), ELogVerbosity::Warning);
		}
	}

	// Fail immediately
	OnFailure.Broadcast();
}

void UCustomCreateSessionCallbackProxy::OnCreateCompleted(FName SessionName, bool bWasSuccessful)
{
	FCunstomOnlineSubsystemBPCallHelper Helper(TEXT("CreateSessionCallback"), WorldContextObject);
	Helper.QueryIDFromPlayerController(PlayerControllerWeakPtr.Get());

	if (Helper.IsValid())
	{
		auto Sessions = Helper.OnlineSub->GetSessionInterface();
		if (Sessions.IsValid())
		{
			Sessions->ClearOnCreateSessionCompleteDelegate_Handle(CreateCompleteDelegateHandle);

			if (bWasSuccessful)
			{
				StartCompleteDelegateHandle = Sessions->AddOnStartSessionCompleteDelegate_Handle(StartCompleteDelegate);
				Sessions->StartSession(NAME_GameSession);

				return;
			}
		}
	}

	if (!bWasSuccessful)
	{
		OnFailure.Broadcast();
	}
}

void UCustomCreateSessionCallbackProxy::OnStartCompleted(FName SessionName, bool bWasSuccessful)
{
	FCunstomOnlineSubsystemBPCallHelper Helper(TEXT("StartSessionCallback"), WorldContextObject);
	Helper.QueryIDFromPlayerController(PlayerControllerWeakPtr.Get());

	if (Helper.IsValid())
	{
		auto Sessions = Helper.OnlineSub->GetSessionInterface();
		if (Sessions.IsValid())
		{
			Sessions->ClearOnStartSessionCompleteDelegate_Handle(StartCompleteDelegateHandle);
		}
	}

	if (bWasSuccessful)
	{
		OnSuccess.Broadcast();
	}
	else
	{
		OnFailure.Broadcast();
	}
}

FCunstomOnlineSubsystemBPCallHelper::FCunstomOnlineSubsystemBPCallHelper(const TCHAR* CallFunctionContext, UObject* WorldContextObject, FName SystemName)
	: OnlineSub(Online::GetSubsystem(GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::ReturnNull), SystemName))
	, FunctionContext(CallFunctionContext)
{
	if (OnlineSub == nullptr)
	{
		FFrame::KismetExecutionMessage(*FString::Printf(TEXT("%s - Invalid or uninitialized OnlineSubsystem"), FunctionContext), ELogVerbosity::Warning);
	}
}

void FCunstomOnlineSubsystemBPCallHelper::QueryIDFromPlayerController(APlayerController* PlayerController)
{
	UserID.Reset();

	if (APlayerState* PlayerState = (PlayerController != NULL) ? PlayerController->PlayerState : NULL)
	{
		UserID = PlayerState->UniqueId.GetUniqueNetId();
		if (!UserID.IsValid())
		{
			FFrame::KismetExecutionMessage(*FString::Printf(TEXT("%s - Cannot map local player to unique net ID"), FunctionContext), ELogVerbosity::Warning);
		}
	}
	else
	{
		FFrame::KismetExecutionMessage(*FString::Printf(TEXT("%s - Invalid player state"), FunctionContext), ELogVerbosity::Warning);
	}
}
