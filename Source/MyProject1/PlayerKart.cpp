#include "PlayerKart.h"

#include "Components/InputComponent.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Net/UnrealNetwork.h"

// Sets default values
APlayerKart::APlayerKart()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	SetReplicateMovement(false);

	MovementComponent = CreateDefaultSubobject<UPlayerCartMovement>(TEXT("MovementComponent"));
	MovementReplicator = CreateDefaultSubobject<UPlayerCartReplicator>(TEXT("MovementReplicator"));
}

// Called when the game starts or when spawned
void APlayerKart::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		NetUpdateFrequency = 1;
	}
}	

FString GetEnumText(ENetRole Role)
{
	switch (Role)
	{
	case ROLE_None:
		return "None";
	case ROLE_SimulatedProxy:
		return "SimulatedProxy";
	case ROLE_AutonomousProxy:
		return "AutonomousProxy";
	case ROLE_Authority:
		return "Authority";
	default:
		return "Error";
	}
}
void APlayerKart::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ENetRole Roles = GetLocalRole();
	if(Roles == ROLE_Authority)
		DrawDebugString(GetWorld(), FVector(0, 0, 200),TEXT("Host") , this, FColor::White, DeltaTime);
	else
		DrawDebugString(GetWorld(), FVector(0, 0, 200), TEXT("Client"), this, FColor::White, DeltaTime);
}

void APlayerKart::SetUp()
{
	UWorld* World = GetWorld();
	APlayerController* PlayerController = World->GetFirstPlayerController();
	FInputModeGameOnly InputModedata;
	PlayerController->SetInputMode(InputModedata);
}


void APlayerKart::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{	
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerKart::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerKart::MoveRight);

}

void APlayerKart::MoveForward(float Value)
{
	if (MovementComponent == nullptr) return;
	MovementComponent->SetThrottle(Value);
}

void APlayerKart::MoveRight(float Value)
{
	if (MovementComponent == nullptr) return;
	MovementComponent->SetSteeringThrow(Value);
}