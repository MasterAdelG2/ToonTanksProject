// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PawnBase.generated.h"

class UCapsuleComponent;
class AProjectileBase;
class UParticleSystem;
class UHealthComponent;
class USoundBase;
class UMatineeCameraShake;
UCLASS()
class TOONTANKS_API APawnBase : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APawnBase();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void HandleDestruction();

	float GetDefaultHealth() { return DefaultHealth;};

	float GetProjectileDamage() {return ProjectileDamage;};
	float GetProjectileSpeed() {return ProjectileSpeed;};

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UFUNCTION(Server, Reliable)
	void Fire();

	bool Reloading=false;

	UPROPERTY(EditAnywhere)
	float RotateTankSpeed= 100.f;

	UPROPERTY(EditAnywhere)
	float RotateTurretSpeed= 1.f;

	UPROPERTY(EditAnywhere)
	float DefaultHealth = 200.f;

	UPROPERTY(EditAnywhere)
	float MoveSpeed = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly,BlueprintReadOnly)
	float ProjectileSpeed = 1300.f;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,BlueprintReadOnly)
	float ProjectileDamage = 50.f;

	UPROPERTY(EditAnywhere)
	float Range = 1.f;

	UPROPERTY(EditAnywhere)
	float ReloadDelay = 1.f;

	UPROPERTY(EditAnywhere)
	float MissingValue = 1.f;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category ="Components",meta=(AllowPrivateAccess="true"))
	UStaticMeshComponent* TurretMesh;

private: 
	// COMPONENTS
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category ="Components",meta=(AllowPrivateAccess="true"))
	UCapsuleComponent* CapsuleComp;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category ="Components",meta=(AllowPrivateAccess="true"))
	UStaticMeshComponent* BaseMesh;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category ="Components",meta=(AllowPrivateAccess="true"))
	USceneComponent* ProjectileSpawnPoint;
	// VARIABLES
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile Type",meta = (AllowPrivateAccess="true"))
	TSubclassOf<AProjectileBase> ProjectileClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile Type",meta = (AllowPrivateAccess="true"))
	UHealthComponent* HealthComponent;

	UPROPERTY(EditAnyWhere,Category = "Effects")
	UParticleSystem* DeathParticle;

	UPROPERTY(EditAnywhere,Category="Effects")
	USoundBase* DeathSound;

	UPROPERTY(EditAnywhere,Category="Effects")
	TSubclassOf<UMatineeCameraShake> DeathShake;
	
	void FinishedReloading();
};
