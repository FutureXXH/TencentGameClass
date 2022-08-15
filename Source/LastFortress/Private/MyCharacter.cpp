// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"
#include <Runtime/Engine/Public/DrawDebugHelpers.h>

// Sets default values
AMyCharacter::AMyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ScaleZ = GetActorTransform().GetScale3D().Z;
	UCapsuleComponent* p = this->FindComponentByClass<UCapsuleComponent>();
	HalfHeight = p->GetScaledCapsuleHalfHeight();
	IKTranceDistance = HalfHeight * ScaleZ;
}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float AMyCharacter::IKFootTrace(float TraceDistance, FName SocketName)
{
	auto SocketPos = GetMesh()->GetSocketLocation(SocketName);
	auto ActorPos = GetActorLocation();
	FVector StartPos;
	FVector EndPos;

	StartPos.X = SocketPos.X;
	StartPos.Y = SocketPos.Y;
	StartPos.Z = ActorPos.Z+ HalfHeight;

	EndPos.X = SocketPos.X;
	EndPos.Y = SocketPos.Y;
	EndPos.Z = ActorPos.Z - TraceDistance;
	FHitResult HitResult;


	FCollisionQueryParams QParams(FName(TEXT("CombatTrace")), true, NULL);
	QParams.AddIgnoredActor(this);

	if(!GetWorld()->LineTraceSingleByChannel(HitResult,StartPos, EndPos, ECC_Visibility))
	{
		//DrawDebugLine(this->GetWorld(), StartPos, EndPos, FColor::Green, true, 5.0f);
		return 0;
	}

	//DrawDebugLine(this->GetWorld(), HitResult.TraceStart, HitResult.TraceEnd, FColor::Red, true, 5.0f);


	
	return HalfHeight - HitResult.Distance;

}

