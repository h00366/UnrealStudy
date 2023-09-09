#include "PlayerKart.h"

#include "Components/InputComponent.h"
#include "Engine/World.h"
#include "Engine/Engine.h"  
#include "DrawDebugHelpers.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "PuzzlePlatformsGameInstance.h"


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
	else
	{

	}

	UPuzzlePlatformsGameInstance* GameInstance = Cast<UPuzzlePlatformsGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GameInstance)
	{
		PlayerIndex = GameInstance->GetPlayerIndex(Cast<APlayerController>(GetController()));
		MovementComponent->SetPNumber(PlayerIndex);
	}	
}


void APlayerKart::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ENetRole Roles = GetLocalRole();
	FString PlayerIndexString = FString::Printf(TEXT("%u"), PlayerIndex);
	if (Roles == ROLE_Authority)
	{
		DrawDebugString(GetWorld(), FVector(0, 0, 200), TEXT("Authority"), this, FColor::White, DeltaTime);
		DrawDebugString(GetWorld(), FVector(0, 0, 300), PlayerIndexString, this, FColor::White, DeltaTime);
	} 
	else if (Roles == ROLE_AutonomousProxy)
	{
		DrawDebugString(GetWorld(), FVector(0, 0, 200), TEXT("AutonomousProxy"), this, FColor::White, DeltaTime);
		DrawDebugString(GetWorld(), FVector(0, 0, 300), PlayerIndexString, this, FColor::White, DeltaTime);
	}
	else if (Roles == ROLE_SimulatedProxy)
	{
		DrawDebugString(GetWorld(), FVector(0, 0, 200), TEXT("SimulatedProxy"), this, FColor::White, DeltaTime);
		DrawDebugString(GetWorld(), FVector(0, 0, 300), PlayerIndexString, this, FColor::White, DeltaTime);
	}
	else
	{
		DrawDebugString(GetWorld(), FVector(0, 0, 200), TEXT("ErrorErrorError"), this, FColor::White, DeltaTime);
		DrawDebugString(GetWorld(), FVector(0, 0, 300), PlayerIndexString, this, FColor::White, DeltaTime);
	}
}

void APlayerKart::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	ENetRole Roles = GetLocalRole();
	if (Roles == ROLE_Authority)
	{
		UE_LOG(LogTemp, Warning, TEXT("서버에서 실행 중"));
		// 서버에서 실행 중
		// ...
	}
	else if (Roles == ROLE_AutonomousProxy)
	{
		UE_LOG(LogTemp, Warning, TEXT("클라이언트에서 실행 중"));
		// 클라이언트에서 실행 중
		// ...
	}
	if (PlayerInputComponent == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("return"));
		return;
	}
	if (HasAuthority())
	{
		PlayerInputComponent->BindAxis("MoveForward", this, &APlayerKart::MoveForward);
		PlayerInputComponent->BindAxis("MoveRight", this, &APlayerKart::MoveRight);
	}
	else
	{
		PlayerInputComponent->BindAxis("MoveForward", this, &APlayerKart::MoveForward);
		PlayerInputComponent->BindAxis("MoveRight", this, &APlayerKart::MoveRight);
	}

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
