// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PawnBase.h"
#include "PawnTank.generated.h"

class USpringArmComponent;
class UCameraComponent;

UCLASS()
class TOONTANKS_API APawnTank : public APawnBase
{
	GENERATED_BODY()
private:

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category ="Components",meta=(AllowPrivateAccess="true"))
	USpringArmComponent* SpringArm;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category ="Components",meta=(AllowPrivateAccess="true"))
	UCameraComponent* Camera;

	
	float MoveThrottle;
	
	float RotateTurretThrottle;
	
	float TurnTankThrottle;

	APlayerController* PlayerControllerRef;
	bool bIsPlayerAlive = true;

	void CalculateMoveInput(float Value);

	void CalculateTurnInput(float Value);

	void CalculateRotateTurretInput(float Value);

	UFUNCTION(Server, Reliable)
	void Server_CalculateMoveInput(float Value);
	UFUNCTION(Server, Reliable)
	void Server_CalculateTurnInput(float Value);
	UFUNCTION(Server, Reliable)
	void Server_CalculateRotateTurretInput(float Value);

	void ApplyMovement(float DeltaTime);

	UPROPERTY(Replicated)
	FTransform TankNewTransform;
	
/* 	void ApplyMove(float DeltaTime);
	
	void ApplyTurn(float DeltaTime);
	
	void ApplyRotateTankTurret(float DeltaTime); */

public:
	// Sets default values for this pawn's properties
	APawnTank();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void HandleDestruction() override;

	bool GetIsPlayerAlive();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


};
