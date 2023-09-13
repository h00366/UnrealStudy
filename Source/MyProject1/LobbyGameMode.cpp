 // Fill out your copyright notice in the Description page of Project Settings.

#include "GameFramework/PlayerController.h"

#include "LobbyGameMode.h"
#include "TimerManager.h"
#include "PuzzlePlatformsGameInstance.h"
#include "GameFramework/PlayerState.h"


void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
    UE_LOG(LogTemp, Warning, TEXT("NumberOfPlayerTest"));
    Super::PostLogin(NewPlayer);

    // Assign a unique player index to each player based on their controller ID
    auto GameInstance = Cast<UPuzzlePlatformsGameInstance>(GetGameInstance());
    if (GameInstance)
    {
        if (GameInstance->PlayerIndexes.Num() < GameInstance->MaxPlayers)
        {
            int32 NewPlayerIndex = GameInstance->PlayerIndexes.Add(NewPlayer->PlayerState->PlayerId);
            UE_LOG(LogTemp, Warning, TEXT("New player index: %d"), NewPlayerIndex);
        }
    }

    if (GameInstance && GameInstance->PlayerIndexes.Num() >= 2)
    {
        GetWorldTimerManager().SetTimer(GameStartTimer, this, &ALobbyGameMode::StartGame, 10);
        UE_LOG(LogTemp, Warning, TEXT("NumberOfPlayer >= 2"));
    }
}

void ALobbyGameMode::Logout(AController* Exiting)
{
    Super::Logout(Exiting);

    auto GameInstance = Cast<UPuzzlePlatformsGameInstance>(GetGameInstance());
    if (GameInstance)
    {
        int32 PlayerIndex = GameInstance->GetPlayerIndex(Cast<APlayerController>(Exiting));
        if (PlayerIndex != -1)
        {
            GameInstance->RemovePlayerIndex(PlayerIndex);
        }
    }
}

void ALobbyGameMode::StartGame()
{
	auto GameInstence = Cast<UPuzzlePlatformsGameInstance> (GetGameInstance());

	if (GameInstence == nullptr) return;

	GameInstence->StartSession();

	UWorld* World = GetWorld();
	bUseSeamlessTravel = true;
	World->ServerTravel("/Game/Map/KartMap?listen");
}
