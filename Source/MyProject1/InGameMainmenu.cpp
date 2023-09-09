// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameMainmenu.h"

#include "Components/Button.h" 
#include "Components/WidgetSwitcher.h" 
#include "Components/EditableTextBox.h"

bool UInGameMainmenu::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) return false;

	CancelButton->OnClicked.AddDynamic(this, &UInGameMainmenu::CancelPressed);

	QuitButton->OnClicked.AddDynamic(this, &UInGameMainmenu::QuitPressed);

	return true;
}

void UInGameMainmenu::CancelPressed()
{
	Teardown();
}

void UInGameMainmenu::QuitPressed()
{
	if (MainMenuInterface != nullptr)
	{
		Teardown(); 
		MainMenuInterface->LoadMainMenu();
	}
}