// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MenuWidget.h"
#include "MainManu.generated.h"	  



USTRUCT()
struct FServerData
{
	GENERATED_BODY()

	FString Name;
	uint16 CurrentPlayers;
	uint16 MaxPlayer;
	FString HostUsername; 

};

/**
 * 
 */
UCLASS()
class MYPROJECT1_API UMainManu : public UMenuWidget
{
	GENERATED_BODY()

public:
	 UMainManu(const FObjectInitializer& FObjectInitializer);

	 void SetServerList(TArray<FServerData> servername);

	 void SelectIndex(uint32 Index);	  

protected:
		virtual bool Initialize() override;

private:

	TSubclassOf<class UUserWidget> ServerRowClass;

	UPROPERTY(meta = (BindWidget))
		class UButton* HostButton;

	UPROPERTY(meta = (BindWidget))
		class UButton* ConfirmHostButton;

	UPROPERTY(meta = (BindWidget))
		class UButton* CancelHostButton;

	UPROPERTY(meta = (BindWidget))
		class UButton* JoinButton;

	UPROPERTY(meta = (BindWidget))
		class UWidgetSwitcher* ManuSwitcher;

	UPROPERTY(meta = (BindWidget))
		class UEditableTextBox* ServerHostName;

	UPROPERTY(meta = (BindWidget))
		class UPanelWidget* ServerList;

	UPROPERTY(meta = (BindWidget))
		class UButton* ConfirmJoinButton_1;

	UPROPERTY(meta = (BindWidget))
		class UButton* ConfirmQeitButton;

	UFUNCTION()
		void HostServer();
	UFUNCTION()
		void JoinServer();
	UFUNCTION()
		void OpenHoseMenu();
	UFUNCTION()
		void OpenJoinServer();
	UFUNCTION()
		void OpenMainMenu();

	UPROPERTY(meta = (BindWidget))
		class UWidget* JoinManu;

	UPROPERTY(meta = (BindWidget))
		class UWidget* HostMenu;

	TOptional<uint32> selectedIndex;
	void UpdateChildren();
	
};  
 