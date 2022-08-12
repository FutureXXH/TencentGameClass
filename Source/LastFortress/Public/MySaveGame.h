// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "MySaveGame.generated.h"


USTRUCT(BlueprintType)
struct FSaveData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadWrite)
	FString PlayerName;

	UPROPERTY(BlueprintReadWrite)
	int32 PlayerScore;


};

/**
 * 
 */
UCLASS()
class LASTFORTRESS_API UMySaveGame : public USaveGame
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite)
	TMap<FString, FSaveData> SaveDatas;


	
	
	UFUNCTION(BlueprintCallable)
	TArray<FSaveData> GetSortSaveDatas();
};
