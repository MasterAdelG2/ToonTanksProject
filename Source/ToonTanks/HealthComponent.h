// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"
class ATankGameModeBase;
class APawnBase;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TOONTANKS_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

private:

	UPROPERTY(EditAnywhere)
	float DefaultHealth;

	float CurruentHealth;
	ATankGameModeBase* GameModeRef;

	APawnBase* Owner;

public:	
	// Sets default values for this component's properties
	UHealthComponent();

	UFUNCTION(BlueprintPure)
	float GetHealth() const;
	UFUNCTION(BlueprintPure)
	float GetMaxHealth() const;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	UFUNCTION()
	void TakeDamage(AActor* DamageActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);
};
