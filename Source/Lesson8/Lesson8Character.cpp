// Copyright Epic Games, Inc. All Rights Reserved.

#include "Lesson8Character.h"

#include <string>

#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"

//////////////////////////////////////////////////////////////////////////
// ALesson8Character

ALesson8Character::ALesson8Character()
{

	bReplicates = true;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)


	//初始化投射物类
	ProjectileClass = AMyProjectile::StaticClass();
	//初始化射速
	FireRate = 0.25f;
	bIsFiringWeapon = false;

	
	OnTakeAnyDamage.AddDynamic(this, &ALesson8Character::AnyDamage);

	
}

void ALesson8Character::BeginPlay()
{
	Super::BeginPlay();

	EnableInput(Cast<APlayerController>(GetController()));

	Weapon = GetWorld()->SpawnActor<AWeapon>(GetActorLocation(), FRotator(0, 0, 0));
	Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, "hand_rSocket");

}

void ALesson8Character::StartFire()
{
	if (!bIsFiringWeapon)
	{
		bIsFiringWeapon = true;
		UWorld* World = GetWorld();
		World->GetTimerManager().SetTimer(FiringTimer, this, &ALesson8Character::StopFire, FireRate, false);
		HandleFire();
	}
}

void ALesson8Character::StopFire()
{
	bIsFiringWeapon = false;
}

void ALesson8Character::HandleFire_Implementation()
{

	if(!IsValid(Weapon))
	{
		return;
	}
	FVector spawnLocation = Weapon->SkeletalMesh->GetSocketTransform("muzzle").GetLocation() ;
	FRotator spawnRotation = GetActorRotation();

	FActorSpawnParameters spawnParameters;
	spawnParameters.Instigator = GetInstigator();
	spawnParameters.Owner = this;

	AMyProjectile* spawnedProjectile = GetWorld()->SpawnActor<AMyProjectile>(spawnLocation, spawnRotation, spawnParameters);
}

void ALesson8Character::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//复制当前生命值。
	DOREPLIFETIME(ALesson8Character, MaxHealth);
	DOREPLIFETIME(ALesson8Character, CurHealth);
}

//////////////////////////////////////////////////////////////////////////
// Input

void ALesson8Character::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &ALesson8Character::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ALesson8Character::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ALesson8Character::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ALesson8Character::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &ALesson8Character::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &ALesson8Character::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &ALesson8Character::OnResetVR);

	// 处理发射投射物
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ALesson8Character::StartFire);
}

void ALesson8Character::AnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	CurHealth -= Damage;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::FromInt(CurHealth));
	if (CurHealth <= 0 && !IsDead)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Death"));
		IsDead = true;

		EnableDeathPhy();

		Cast<AMyPlayerState>(InstigatedBy->PlayerState)->KillCount++;
		Cast<AMyPlayerState>(GetController()->PlayerState)->DeathCount++;

		GetWorld()->GetTimerManager().SetTimer(DeathTimer, this, &ALesson8Character::Death, 5, false);

	}

}




void ALesson8Character::Death_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Respawn"));
	auto TempController = GetController();
	DeathClear();
	Destroy();
	Cast<ALesson8GameMode>(GetWorld()->GetAuthGameMode())->RestartPlayer(TempController);
}

void ALesson8Character::EnableDeathPhy_Implementation()
{
	
	DisableInput(Cast<APlayerController>(GetController()));

	GetMesh()->SetCollisionProfileName(TEXT("Pawn"));
	GetMesh()->SetAllBodiesSimulatePhysics(true);
	GetMesh()->SetAllBodiesPhysicsBlendWeight(1, false);

}

void ALesson8Character::DeathClear_Implementation()
{

	Weapon->Destroy();
}


void ALesson8Character::OnResetVR()
{
	// If Lesson8 is added to a project via 'Add Feature' in the Unreal Editor the dependency on HeadMountedDisplay in Lesson8.Build.cs is not automatically propagated
	// and a linker error will result.
	// You will need to either:
	//		Add "HeadMountedDisplay" to [YourProject].Build.cs PublicDependencyModuleNames in order to build successfully (appropriate if supporting VR).
	// or:
	//		Comment or delete the call to ResetOrientationAndPosition below (appropriate if not supporting VR)
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void ALesson8Character::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
		Jump();
}

void ALesson8Character::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
		StopJumping();
}

void ALesson8Character::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ALesson8Character::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ALesson8Character::MoveForward(float Value)
{
	
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ALesson8Character::MoveRight(float Value)
{
	if ( (Controller != nullptr) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

