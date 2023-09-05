// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameModeBase.h"
#include "MyProject1GameMode.h"
#include "PCarGameMode.generated.h"



UCLASS()
class MYPROJECT1_API APCarGameMode : public AMyProject1GameMode
{
	GENERATED_BODY()
		
public:
	APCarGameMode();

	virtual void PostLogin(APlayerController* NewPlayer) override;

	virtual void Logout(AController* Exiting) override;
};
