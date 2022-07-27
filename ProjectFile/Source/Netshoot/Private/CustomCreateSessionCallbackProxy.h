// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Net/OnlineBlueprintCallProxyBase.h"
#include "../Plugins/Online/OnlineSubsystem/Source/Public/Interfaces/OnlineSessionInterface.h"
#include "../Plugins/Online/OnlineSubsystem/Source/Public/OnlineSubsystem.h"
#include "CustomCreateSessionCallbackProxy.generated.h"


struct FCunstomOnlineSubsystemBPCallHelper
{
public:
	FCunstomOnlineSubsystemBPCallHelper(const TCHAR* CallFunctionContext, UObject* WorldContextObject, FName SystemName = NAME_None);

	void QueryIDFromPlayerController(APlayerController* PlayerController);

	bool IsValid() const
	{
		return true;
	}

public:
	TSharedPtr<const FUniqueNetId> UserID;
	IOnlineSubsystem* const OnlineSub;
	const TCHAR* FunctionContext;
};


UCLASS()
class UCustomCreateSessionCallbackProxy : public UOnlineBlueprintCallProxyBase
{
	GENERATED_UCLASS_BODY()

		// Called when the session was created successfully
	UPROPERTY(BlueprintAssignable)
	FEmptyOnlineDelegate OnSuccess;

	// Called when there was an error creating the session
	UPROPERTY(BlueprintAssignable)
	FEmptyOnlineDelegate OnFailure;

	// Creates a session with the default online subsystem
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject"), Category = "Online|Session")
	static UCustomCreateSessionCallbackProxy* CreateCustomSession(UObject* WorldContextObject, class APlayerController* PlayerController, int32 PublicConnections, bool bUseLAN);

	// UOnlineBlueprintCallProxyBase interface
	virtual void Activate() override;
	// End of UOnlineBlueprintCallProxyBase interface

private:
	// Internal callback when session creation completes, calls StartSession
	void OnCreateCompleted(FName SessionName, bool bWasSuccessful);

	// Internal callback when session creation completes, calls StartSession
	void OnStartCompleted(FName SessionName, bool bWasSuccessful);

	// The player controller triggering things
	TWeakObjectPtr<APlayerController> PlayerControllerWeakPtr;

	// The delegate executed by the online subsystem
	FOnCreateSessionCompleteDelegate CreateCompleteDelegate;

	// The delegate executed by the online subsystem
	FOnStartSessionCompleteDelegate StartCompleteDelegate;

	// Handles to the registered delegates above
	FDelegateHandle CreateCompleteDelegateHandle;
	FDelegateHandle StartCompleteDelegateHandle;

	// Number of public connections
	int NumPublicConnections;

	// Whether or not to search LAN
	bool bUseLAN;

	// The world context object in which this call is taking place
	UObject* WorldContextObject;
};
