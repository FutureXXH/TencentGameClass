// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBlueprintFunctionLibrary.h"



FHitResult UMyBlueprintFunctionLibrary::MyLineTraceSingle(AActor* Actor,FVector Start, FVector End, ECollisionChannel TraceChannel)
{
	

	FHitResult HitResult;
	if(!IsValid(Actor))
	{
		return HitResult;
	}
	TArray<AActor*> IgnoreActors;
	bool bIsHit = UKismetSystemLibrary::LineTraceSingle(Actor->GetWorld(), Start, End, TraceTypeQuery2, false, IgnoreActors, EDrawDebugTrace::ForDuration, HitResult, true);


	

	return HitResult;
}
