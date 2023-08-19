// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuWidget.h"

void UMenuWidget::SetUp()
{

	this->AddToViewport();

	UWorld* World = GetWorld();
	APlayerController* PlayerController = World->GetFirstPlayerController();

	UE_LOG(LogTemp, Warning, TEXT("SetUp"));

	FInputModeUIOnly	InputModData;
	InputModData.SetWidgetToFocus(this->TakeWidget());
	InputModData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

	PlayerController->SetInputMode(InputModData);

	PlayerController->bShowMouseCursor = true;

}

void UMenuWidget::Teardown()
{
	this->RemoveFromViewport();

	UWorld* World = GetWorld();

	APlayerController* PlayerController = World->GetFirstPlayerController();
	 
	FInputModeGameOnly InputModedata;
	PlayerController->SetInputMode(InputModedata);
	PlayerController->bShowMouseCursor = false;
}
void UMenuWidget::SetMenuInterface(IMainMenuInterface* MainMenuInterfaces)
{
	this->MainMenuInterface = MainMenuInterfaces;
}
