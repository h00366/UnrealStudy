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
	// 플레이어 인덱스를 SetUp 함수에 전달
}

// Called when the game starts or when spawned
void APlayerKart::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		NetUpdateFrequency = 1;
	}
	UPuzzlePlatformsGameInstance* GameInstance = Cast<UPuzzlePlatformsGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GameInstance)
	{
		int32 PlayerIndex = GameInstance->PlayerIndex-1;
		// 나머지 코드 실행
		SetUp(0);
		UE_LOG(LogTemp, Warning, TEXT("%d"), 0);
	}
	else
		UE_LOG(LogTemp, Warning, TEXT("ErrorError"));
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
		DrawDebugString(GetWorld(), FVector(0, 0, 200),TEXT("Authority") , this, FColor::White, DeltaTime);
	else if (Roles == ROLE_AutonomousProxy)
		DrawDebugString(GetWorld(), FVector(0, 0, 200), TEXT("AutonomousProxy"), this, FColor::White, DeltaTime);
	else
		DrawDebugString(GetWorld(), FVector(0, 0, 200), TEXT("SimulatedProxy"), this, FColor::White, DeltaTime);

}

void APlayerKart::SetUp(int32 PlayerIndex)
{
	UWorld* World = GetWorld();
	if (World)
	{	
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(World, PlayerIndex);
		if (PlayerController)
		{
			// 입력 모드를 재설정하여 게임과 UI 입력을 허용합니다.
			FInputModeGameAndUI InputModeData;
			PlayerController->SetInputMode(InputModeData);
		}
	}
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