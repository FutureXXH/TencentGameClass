// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Interfaces/OnlineSessionInterface.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "OnlineSessionSettings.h"
#include "MyGameInstanceSubsystem.generated.h"

USTRUCT(BlueprintType)
struct FMySessionSearchResult
{
	GENERATED_USTRUCT_BODY()

	
		FOnlineSessionSearchResult SessionSearchResult;

	UPROPERTY(BlueprintReadWrite)
		FString SessionName;
	UPROPERTY(BlueprintReadWrite)
		FString Password;
	FMySessionSearchResult()
	{

	}

	FMySessionSearchResult(FOnlineSessionSearchResult& Res, FString Name, FString password)
	{
		SessionSearchResult = Res;
		SessionName = Name;
		Password = password;
	}
};



DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCSOnCreateSessionComplete, bool, Successful);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCSOnEndSessionComplete, bool, Successful);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCSOnStartSessionComplete, bool, Successful);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCSOnDestroySessionComplete, bool, Successful);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FCSOnFindSessionsComplete,const TArray<FMySessionSearchResult>&, SessionResults, bool, Successful);

DECLARE_MULTICAST_DELEGATE_OneParam(FCSOnJoinSessionComplete, EOnJoinSessionCompleteResult::Type Result);

/**
 * 
 */






UCLASS()
class LASTFORTRESS_API UMyGameInstanceSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	UMyGameInstanceSubsystem();
	//create
	UFUNCTION(BlueprintCallable, Category = GameInstanceSubsystem)
	void CreateSession(int32 NumPublicConnections, bool IsLANMatch, FString MapName);

	UPROPERTY(BlueprintAssignable)
	FCSOnCreateSessionComplete OnCreateSessionCompleteEvent;

	//start
	UFUNCTION(BlueprintCallable, Category = GameInstanceSubsystem)
	void StartSession();
	UPROPERTY(BlueprintAssignable)
	FCSOnStartSessionComplete OnStartSessionCompleteEvent;

	//end
	UFUNCTION(BlueprintCallable, Category = GameInstanceSubsystem)
	void EndSession();
	UPROPERTY(BlueprintAssignable)
	FCSOnEndSessionComplete OnEndSessionCompleteEvent;

	//Destroy
	UFUNCTION(BlueprintCallable, Category = GameInstanceSubsystem)
	void DestroySession();
	UPROPERTY(BlueprintAssignable)
	FCSOnDestroySessionComplete OnDestroySessionCompleteEvent;

	//Find 
	UFUNCTION(BlueprintCallable, Category = GameInstanceSubsystem)
	void FindSessions(int32 MaxSearchResults, bool IsLANQuery);
	UPROPERTY(BlueprintAssignable)
	FCSOnFindSessionsComplete OnFindSessionsCompleteEvent;

	//Join
	UFUNCTION(BlueprintCallable, Category = GameInstanceSubsystem)
	void JoinGameSession(const FMySessionSearchResult& MySessionResult);
	//UPROPERTY(BlueprintAssignable)
	FCSOnJoinSessionComplete OnJoinGameSessionCompleteEvent;

protected:
	void OnCreateSessionCompleted(FName SessionName, bool Successful);

	void OnEndSessionCompleted(FName SessionName, bool Successful);

	void OnStartSessionCompleted(FName SessionName, bool Successful);

	void OnDestroySessionCompleted(FName SessionName, bool Successful);

	void OnFindSessionsCompleted(bool Successful);

	void OnJoinSessionCompleted(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
	bool TryTravelToCurrentSession();

private:
	FOnCreateSessionCompleteDelegate CreateSessionCompleteDelegate;
	FDelegateHandle CreateSessionCompleteDelegateHandle;
	TSharedPtr<FOnlineSessionSettings> LastSessionSettings;

	FOnStartSessionCompleteDelegate StartSessionCompleteDelegate;
	FDelegateHandle StartSessionCompleteDelegateHandle;

	FOnEndSessionCompleteDelegate EndSessionCompleteDelegate;
	FDelegateHandle EndSessionCompleteDelegateHandle;

	FOnDestroySessionCompleteDelegate DestroySessionCompleteDelegate;
	FDelegateHandle DestroySessionCompleteDelegateHandle;

	FOnFindSessionsCompleteDelegate FindSessionsCompleteDelegate;
	FDelegateHandle FindSessionsCompleteDelegateHandle;
	TSharedPtr<FOnlineSessionSearch> LastSessionSearch;

	FOnJoinSessionCompleteDelegate JoinSessionCompleteDelegate;
	FDelegateHandle JoinSessionCompleteDelegateHandle;
};
