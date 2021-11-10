// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileBase.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "particles/ParticleSystemComponent.h"

// Sets default values
AProjectileBase::AProjectileBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile Mesh"));
	RootComponent = ProjectileMesh;

	ParticleTrail = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle Trail"));
	ParticleTrail->SetupAttachment(RootComponent);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement"));
	InitialLifeSpan = 3.f;
	ProjectileMovement->UpdatedComponent = ProjectileMesh;
}

// Called when the game starts or when spawned
void AProjectileBase::BeginPlay()
{
	Super::BeginPlay();
	UGameplayStatics::PlaySoundAtLocation(this,LaunchSound,GetActorLocation());
	ProjectileMesh->OnComponentHit.AddDynamic(this, &AProjectileBase::OnHit);
	ProjectileMovement->InitialSpeed = ProjectileSpeed;
	ProjectileMovement->MaxSpeed = ProjectileSpeed;
}

void AProjectileBase::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) 
{
	// Try to get a reference to the owning class.
	APawn* MyOwner = Cast<APawn>(GetOwner());
	// If for some reason we can't get a valid reference, return as we need to check against the owner. 
	if(!MyOwner)
	{
		UE_LOG(LogTemp,Warning,TEXT("No Owner"));
		return;
	}
	// If the other ISN'T self OR Owner AND exists, then apply damage. 
	if(OtherActor && OtherActor != this && OtherActor != MyOwner)
	{
		UGameplayStatics::ApplyDamage(OtherActor, Damage, MyOwner->GetInstigatorController(), this, DamageType);
		UGameplayStatics::SpawnEmitterAtLocation(this, HitParticle,GetActorLocation());
		UGameplayStatics::PlaySoundAtLocation(this,HitSound,GetActorLocation());
		APlayerController* PController = GetGameInstance()->GetFirstLocalPlayerController();
		if (PController)
			PController->ClientStartCameraShake(HitShake);
	}
	Destroy();
}