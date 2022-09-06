// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Net/UnrealNetwork.h"
#include "Public/MyProjectile.h"
#include "Lesson8GameMode.h"
#include "Public/Weapon.h"
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Lesson8Character.generated.h"

UCLASS(config=Game)
class LESSON8_API ALesson8Character : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
public:
	ALesson8Character();


	virtual void BeginPlay() override;

	/** ���Ը��� */
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly,Replicated)
	int MaxHealth = 10;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Replicated)
	int CurHealth = 10;

	UPROPERTY(VisibleAnywhere)
	bool IsDead = false;

	UPROPERTY(EditAnywhere)
		bool JumpButtonDown;
	UPROPERTY(EditAnywhere)
		bool CrouchButtonDown;
	UPROPERTY(VisibleAnywhere)
		AWeapon* Weapon;

	

protected:

	/** Resets HMD orientation in VR. */
	void OnResetVR();

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);


	UPROPERTY(EditDefaultsOnly, Category = "Gameplay|Projectile")
		TSubclassOf<class AMyProjectile> ProjectileClass;

	/** ���֮����ӳ٣���λΪ�롣���ڿ��Ʋ��Է����������ٶȣ����ɷ�ֹ������������������½�SpawnProjectileֱ�Ӱ������롣*/
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
		float FireRate;

	/** ��Ϊtrue�������ڷ���Ͷ���*/
	bool bIsFiringWeapon;

	/** ����������������ĺ�����*/
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
		void StartFire();

	/** ���ڽ�����������ĺ�����һ��������δ��룬��ҿ��ٴ�ʹ��StartFire��*/
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
		void StopFire();

	/** ��������Ͷ����ķ�����������*/
	UFUNCTION(Server, Reliable)
		void HandleFire();
	    void HandleFire_Implementation();

	UFUNCTION(BlueprintCallable, Server,Reliable)
	    void Death();
	    void Death_Implementation();

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
			void EnableDeathPhy();
		    void EnableDeathPhy_Implementation();

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
	         void DeathClear();
			void DeathClear_Implementation();


	/** ��ʱ������������ṩ���ɼ��ʱ���ڵ������ӳ١�*/
	FTimerHandle FiringTimer;

	FTimerHandle DeathTimer;



protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	UFUNCTION()
	void AnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);
	



};

