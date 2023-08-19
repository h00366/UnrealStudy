// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MenuWidget.h"
#include "MainManu.generated.h"	  



/**
 * 
 */
UCLASS()
class MYPROJECT1_API UMainManu : public UMenuWidget
{
	GENERATED_BODY()

public:
	 UMainManu(const FObjectInitializer& FObjectInitializer);

	 void SetServerList(TArray<FString> servername);

	 void SelectIndex(uint32 Index);	  

protected:
		virtual bool Initialize() override;

private:

	TSubclassOf<class UUserWidget> ServerRowClass;

	UPROPERTY(meta = (BindWidget))
		class UButton* HostButton;

	UPROPERTY(meta = (BindWidget))
		class UButton* JoinButton;

	UPROPERTY(meta = (BindWidget))
		class UWidgetSwitcher* ManuSwitcher;

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
		void OpenJoinServer();

	UPROPERTY(meta = (BindWidget))
		class UWidget* JoinManu;

	TOptional<uint32> selectedIndex;
	
};  
