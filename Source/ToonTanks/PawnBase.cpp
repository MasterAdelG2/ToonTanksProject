// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnBase.h"
#include "Components/CapsuleComponent.h"
#include "ToonTanks/ProjectileBase.h"
#include "Kismet/GameplayStatics.h"
#include "HealthComponent.h"
#include "TimerManager.h"
// Sets default values
APawnBase::APawnBase()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Collider"));
	RootComponent = CapsuleComp;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	BaseMesh->SetupAttachment(RootComponent);

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret Mesh"));
	TurretMesh->SetupAttachment(BaseMesh);

	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Projectile Spawn Point"));
	ProjectileSpawnPoint->SetupAttachment(TurretMesh);

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health Component"));

}

// Called when the game starts or when spawned
void APawnBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Redirects turret to get shortest rotation
/* FRotator ShortestRotation(FRotator Rotator)
{
	// For Rotating Right
	Rotator.Yaw = Rotator.Yaw > 180 ? Rotator.Yaw-360: Rotator.Yaw;
	Rotator.Pitch = Rotator.Pitch > 180 ? Rotator.Pitch-360: Rotator.Pitch;
	Rotator.Roll = Rotator.Roll > 180 ? Rotator.Roll-360: Rotator.Roll;
	// For Rotating Left
	Rotator.Yaw = Rotator.Yaw < -180 ? Rotator.Yaw+360: Rotator.Yaw;
	Rotator.Pitch = Rotator.Pitch < -180 ? Rotator.Pitch+360: Rotator.Pitch;
	Rotator.Roll = Rotator.Roll < -180 ? Rotator.Roll+360: Rotator.Roll;

	return Rotator;
} */



// For Targetting by Mouse
/* void APawnBase::RotateTurret(FVector LookAtTarget,float DeltaTime) 
{
	FVector LookAtTargetCleaned = FVector(LookAtTarget.X,LookAtTarget.Y,TurretMesh->GetComponentLocation().Z);
	FVector TurretLocation = TurretMesh->GetComponentLocation();
	FRotator TurretRotation = TurretMesh->GetComponentRotation();
	FRotator EndRotation = FVector(LookAtTargetCleaned - TurretLocation).Rotation();
	
	FRotator TurretNeeded = ShortestRotation(EndRotation-TurretRotation) * DeltaTime * RotateTurretSpeed;
	TurretMesh->AddWorldRotation(TurretNeeded);
} */

void APawnBase::Fire() 
{
	if (ProjectileClass && !Reloading)
	{
		FVector SpawnLoc = ProjectileSpawnPoint->GetComponentLocation();
		FRotator SpawnRot = ProjectileSpawnPoint->GetComponentRotation();
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		AProjectileBase* TempProjectile = GetWorld()->SpawnActor<AProjectileBase>(ProjectileClass,SpawnLoc,SpawnRot,SpawnParams); 
		//TempProjectile->Initialize(this,GetController(),SpawnLoc);
		//TempProjectile->SetOwner(this);

		Reloading = true;
		FTimerHandle UnusedHandle;
 		GetWorldTimerManager().SetTimer(UnusedHandle, this, &APawnBase::FinishedReloading, ReloadDelay, false);
	}
}

void APawnBase::FinishedReloading()
{
	Reloading = false;
}

void APawnBase::HandleDestruction() 
{
	// --- Universal functionality --- 
	// Play death effects particle, sound and camera shake. 
	UGameplayStatics::SpawnEmitterAtLocation(this,DeathParticle,GetActorLocation());
	UGameplayStatics::PlaySoundAtLocation(this,DeathSound,GetActorLocation());
	
	GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(DeathShake);

}

// Called every frame
void APawnBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APawnBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

