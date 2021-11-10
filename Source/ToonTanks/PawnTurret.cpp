// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnTurret.h"
#include "Kismet/GameplayStatics.h"

// Called when the game starts or when spawned
void APawnTurret::BeginPlay()
{
	Super::BeginPlay();
	
    GetWorld()->GetTimerManager().SetTimer(FireRateTimer,this,&APawnTurret::CheckFireCondition,FireRate,true);
    PlayerPawn = Cast<APawnTank>(UGameplayStatics::GetPlayerPawn(this,0));
}

void APawnTurret::HandleDestruction() 
{
    Super::HandleDestruction();
    Destroy();
}

// Called every frame
void APawnTurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    
    if (!PlayerPawn || ReturnDistanceToPlayer()> FireRange)
    {
        return;
    }
    RotateCannonTurret(PlayerPawn->GetActorLocation(),DeltaTime);
}

void APawnTurret::RotateCannonTurret(FVector LookAtTarget,float DeltaTime) 
{
	FVector LookAtTargetCleaned = FVector(LookAtTarget.X,LookAtTarget.Y,TurretMesh->GetComponentLocation().Z);
	FVector StartLocation = TurretMesh->GetComponentLocation();
	FRotator TurretRotation = FVector(LookAtTargetCleaned - StartLocation).Rotation();
	TurretMesh->SetWorldRotation(TurretRotation);
}

void APawnTurret::CheckFireCondition() 
{

    if (!PlayerPawn || !PlayerPawn->GetIsPlayerAlive())
    {
        return;
    }
    if (ReturnDistanceToPlayer() <= FireRange)
    {
        Fire();
        return;
    }
}

float APawnTurret::ReturnDistanceToPlayer() 
{
    if (!PlayerPawn)
    {
        return 0.f;
    }
    return FVector::Dist(PlayerPawn->GetActorLocation(),GetActorLocation());
}
