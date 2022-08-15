// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <Components/SkeletalMeshComponent.h>
#include <Components/StaticMeshComponent.h>
#include <Components/CapsuleComponent.h>
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyCharacter.generated.h"


UCLASS()
class LASTFORTRESS_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	



	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	UPROPERTY(BlueprintReadWrite)
	float IKRightFoot;
	UPROPERTY(BlueprintReadWrite)
	float IKLeftFoot;
	UPROPERTY(BlueprintReadWrite)
	float ScaleZ;
	UPROPERTY(BlueprintReadWrite)
	float IKTranceDistance;
	UPROPERTY(BlueprintReadWrite)
	float HalfHeight;

	UFUNCTION(BlueprintCallable)
		float IKFootTrace(float TraceDistance, FName SocketName);
};
