// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnTank.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

APawnTank::APawnTank()
{
    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
    SpringArm->SetupAttachment(RootComponent);

    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera Component"));
    Camera->SetupAttachment(SpringArm);
}

// Called when the game starts or when spawned
void APawnTank::BeginPlay()
{
	Super::BeginPlay();
	
    PlayerControllerRef = Cast<APlayerController>(GetController());
}

void APawnTank::HandleDestruction() 
{
    Super::HandleDestruction();
    bIsPlayerAlive = false;

    SetActorHiddenInGame(true);
    SetActorTickEnabled(false);
}

bool APawnTank::GetIsPlayerAlive() 
{
    return bIsPlayerAlive;
}

// Called every frame
void APawnTank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    Turn(DeltaTime);
    Move(DeltaTime);
    RotateTankTurret(DeltaTime);
}

// Called to bind functionality to input
void APawnTank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
    PlayerInputComponent->BindAxis("MoveForward",this,&APawnTank::CalculateMoveInput);
    PlayerInputComponent->BindAxis("Turn",this,&APawnTank::CalculateTurnInput);
    PlayerInputComponent->BindAction("Fire",IE_Pressed,this,&APawnTank::Fire);
    PlayerInputComponent->BindAxis("RotateTurret",this,&APawnTank::CalculateRotateTurretInput);
}

void APawnTank::CalculateMoveInput(float Value) 
{
    MoveThrottle = Value;
}

void APawnTank::CalculateTurnInput(float Value) 
{
    TurnTankThrottle = Value;
}

void APawnTank::CalculateRotateTurretInput(float Value) 
{
    RotateTurretThrottle = Value;
}

void APawnTank::Move(float DeltaTime) 
{
    FVector AddedDirection = FVector(MoveThrottle*MoveSpeed*DeltaTime,0,0 );
    AddActorLocalOffset(AddedDirection,true);
}

void APawnTank::Turn(float DeltaTime) 
{
    FQuat AddedTurnDirection = FQuat(FRotator(0,TurnTankThrottle * RotateTankSpeed * DeltaTime,0));
    AddActorWorldRotation(AddedTurnDirection,true);
}

void APawnTank::RotateTankTurret(float DeltaTime)
{
    FQuat AddedTurretDirection = FQuat(FRotator(0,RotateTurretThrottle * RotateTurretSpeed * DeltaTime,0));
    TurretMesh->AddWorldRotation(AddedTurretDirection,true);
}