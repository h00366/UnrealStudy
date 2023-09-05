// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuInterface.h"

#include "MenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT1_API UMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetUp();
	void Teardown();

	void SetMenuInterface(IMainMenuInterface* MainMenuInterfaces);

protected:
	IMainMenuInterface* MainMenuInterface;
};
