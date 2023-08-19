// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"

#include "MainMenuInterface.h"
#include "PuzzlePlatformsGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT1_API UPuzzlePlatformsGameInstance : public UGameInstance , public IMainMenuInterface
{
	GENERATED_BODY()
public:
		UPuzzlePlatformsGameInstance(const FObjectInitializer& FObjectInitializer);
		virtual void Init();

		UFUNCTION(BlueprintCallable)
			void LoadMenu();	

		UFUNCTION(BlueprintCallable)
			void InGameLoadMenu();

		UFUNCTION(Exec)
			void Host() override;

		UFUNCTION(Exec)
			void Join(uint32 Index) override;

		virtual void LoadMainMenu() override;
		virtual void RefreshServerList() override;
private:
	TSubclassOf<class UUserWidget>MenuClass; 
	TSubclassOf<class UUserWidget>InGameMenuClass; 


	IOnlineSessionPtr SessionInterface;
	TSharedPtr<class FOnlineSessionSearch> SessionSearch;

	void OnCreateSessionComplete(FName SessionName, bool succesess);
	void OnDestroySessionComplete(FName SessionName, bool succesess);
	void OnFindSessionsComplete(bool succesess);
	void FOnJoinSessionComplete(FName sessionName, EOnJoinSessionCompleteResult::Type Result);

	void CreateSession();

	class UMainManu* Menu;

};
