// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MyProject1HUD.generated.h"

/**
 * 
 */
UCLASS(config = Game)
class MYPROJECT1_API AMyProject1HUD : public AHUD
{
	GENERATED_BODY()

public:
	AMyProject1HUD();

	/** Font used to render the vehicle info */
	UPROPERTY()
		UFont* HUDFont;

	// Begin AHUD interface
	virtual void DrawHUD() override;
	// End AHUD interface
};

