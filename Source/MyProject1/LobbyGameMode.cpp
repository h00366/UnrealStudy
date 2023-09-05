 // Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameMode.h"
#include "TimerManager.h"
#include "PuzzlePlatformsGameInstance.h"

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	UE_LOG(LogTemp, Warning, TEXT("NumberOfPlayerTest"));
	Super::PostLogin(NewPlayer);
	++NumberOfPlayer;
	auto GameInstence = Cast<UPuzzlePlatformsGameInstance>(GetGameInstance());
	GameInstence->PlayerIndex = NumberOfPlayer;

	if (NumberOfPlayer >= 2)
	{
  		GetWorldTimerManager().SetTimer(GameStartTimer,this, &ALobbyGameMode::StartGame,5);
		UE_LOG(LogTemp,Warning ,TEXT("NumberOfPlayer = 3"));
	}
}

void ALobbyGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
	--NumberOfPlayer;
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
