// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"
#include "ToonTanks/TankGameModeBase.h"
#include "ToonTanks/PawnBase.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}

float UHealthComponent::GetHealth() const
{
	return CurruentHealth;
}

float UHealthComponent::GetMaxHealth() const
{
	return DefaultHealth;
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	GameModeRef = Cast<ATankGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::TakeDamage);
	Owner = Cast<APawnBase>(GetOwner());
	if (Owner)
	{
		CurruentHealth = Owner->GetDefaultHealth();
		DefaultHealth = CurruentHealth;
	} 
} 

void UHealthComponent::TakeDamage(AActor* DamageActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser) {
	
	if (Damage ==0 || CurruentHealth <=0)
	{
		return;
	}
	CurruentHealth = FMath::Clamp(CurruentHealth - Damage,0.f,DefaultHealth);

	if (CurruentHealth<=0)
	{
		if (GameModeRef)
		{
			GameModeRef->ActorDied(GetOwner());
		}
		else
		{
			UE_LOG(LogTemp,Warning,TEXT("No GM Ref"));
		}
	}
}


