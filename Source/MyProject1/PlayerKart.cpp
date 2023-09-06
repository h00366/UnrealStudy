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

	// 게임 인스턴스 가져오기
	UPuzzlePlatformsGameInstance* GameInstance = Cast<UPuzzlePlatformsGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GameInstance)
	{
		// 플레이어 인덱스 가져오기
		int32 PlayerIndex = GameInstance->PlayerIndex;

		// 호스트로 인식된 플레이어에게 Pawn을 제어할 수 있도록 코드 추가
		if (HasAuthority() && PlayerIndex == 0)
		{
			SetUp(PlayerIndex);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("ErrorError"));
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
	if (Roles == ROLE_Authority)
		DrawDebugString(GetWorld(), FVector(0, 0, 200), TEXT("Authority"), this, FColor::White, DeltaTime);
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
			APawn* ControlledPawn = PlayerController->GetPawn(); // 플레이어 컨트롤러가 제어하는 폰(캐릭터)을 가져옵니다.

			if (ControlledPawn)
			{
				FString PawnName = ControlledPawn->GetName(); // 폰의 이름을 가져옵니다.
				UE_LOG(LogTemp, Warning, TEXT("현재 제어 중인 폰의 이름: %s"), *PawnName);
			}
			UE_LOG(LogTemp, Warning, TEXT("TestMove : %d"), PlayerIndex);

			PlayerController->SetIgnoreMoveInput(false); // 이동 입력 막음 해제
			PlayerController->SetIgnoreLookInput(false); // 시선 이동 입력 막음 해제
			
			// 입력 모드를 재설정하여 게임과 UI 입력을 허용합니다.
			FInputModeGameAndUI InputModeData;
			PlayerController->SetInputMode(InputModeData);
	
			// Pawn을 컨트롤러에 연결
			PlayerController->Possess(this);
		}
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
