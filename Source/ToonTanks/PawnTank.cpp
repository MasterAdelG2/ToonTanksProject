// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnTank.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Net/UnrealNetwork.h"

APawnTank::APawnTank()
{
    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
    SpringArm->SetupAttachment(RootComponent);

    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera Component"));
    Camera->SetupAttachment(SpringArm);

    PrimaryActorTick.bCanEverTick = true;
    bReplicates = true;
}

// Called when the game starts or when spawned
void APawnTank::BeginPlay()
{
	Super::BeginPlay();
	
    PlayerControllerRef = Cast<APlayerController>(GetController());
}

void APawnTank::GetLifetimeReplicatedProps( TArray< FLifetimeProperty > & OutLifetimeProps ) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(APawnTank, TankNewTransform);
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

    ApplyMovement(DeltaTime);
}

// Called to bind functionality to input
void APawnTank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
    PlayerInputComponent->BindAxis("MoveForward",this,&APawnTank::CalculateMoveInput);
    PlayerInputComponent->BindAxis("Turn",this,&APawnTank::CalculateTurnInput);
    PlayerInputComponent->BindAxis("RotateTurret",this,&APawnTank::CalculateRotateTurretInput);
    PlayerInputComponent->BindAction("Fire",IE_Pressed,this,&APawnTank::Fire);
}

void APawnTank::CalculateMoveInput(float Value) 
{
    MoveThrottle = Value;
    Server_CalculateMoveInput(Value);
}

void APawnTank::CalculateTurnInput(float Value) 
{
    TurnTankThrottle = Value;
    Server_CalculateTurnInput(Value);
}

void APawnTank::CalculateRotateTurretInput(float Value) 
{
    RotateTurretThrottle = Value;
    Server_CalculateRotateTurretInput(Value);
}

void APawnTank::Server_CalculateMoveInput_Implementation(float Value)
{
    MoveThrottle = Value;
}

void APawnTank::Server_CalculateTurnInput_Implementation(float Value)
{
    TurnTankThrottle = Value;
}

void APawnTank::Server_CalculateRotateTurretInput_Implementation(float Value)
{
    RotateTurretThrottle = Value;
}

void APawnTank::ApplyMovement(float DeltaTime)
{
    FVector AddedDirection = FVector(MoveThrottle*MoveSpeed*DeltaTime,0,0 );
    FQuat AddedTurnDirection = FQuat(FRotator(0,TurnTankThrottle * RotateTankSpeed * DeltaTime,0));
    FQuat AddedTurretDirection = FQuat(FRotator(0,RotateTurretThrottle * RotateTurretSpeed * DeltaTime,0));

    AddActorLocalOffset(AddedDirection,true);
    AddActorWorldRotation(AddedTurnDirection,true);
    TurretMesh->AddWorldRotation(AddedTurretDirection,true);

    if (HasAuthority()) 
	{
		TankNewTransform = GetActorTransform();
	}
	else 
	{
        //SetActorTransform(TankNewTransform);
	}
}
