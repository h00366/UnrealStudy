// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyProject1GameMode.h"
#include "LobbyGameMode.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT1_API ALobbyGameMode : public AMyProject1GameMode
{
	GENERATED_BODY()

public:
	ALobbyGameMode();
	virtual void PostLogin(APlayerController* NewPlayer) override;

	virtual void Logout(AController* Exiting) override;

private:

	void StartGame();

	uint32 NumberOfPlayer = 0;

	FTimerHandle GameStartTimer; 

};
