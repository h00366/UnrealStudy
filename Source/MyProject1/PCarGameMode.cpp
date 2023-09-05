// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerKartPown.h"
#include "PCarGameMode.h"
#include "PlayerKart.h"
#include "MyProject1HUD.h"
#include "PuzzlePlatformsGameInstance.h"


APCarGameMode::APCarGameMode()
{

//	FInputModeGameOnly InputModedata;
//	DefaultPawnClass = APlayerKartPown::StaticClass(); 
//	UE_LOG(LogTemp, Warning, TEXT("StartCreate"));
}

void APCarGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

//	FInputModeGameOnly InputModedata;
//	NewPlayer->SetInputMode(InputModedata);

}

void APCarGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
}