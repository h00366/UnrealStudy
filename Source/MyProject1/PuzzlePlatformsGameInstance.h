// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "GameFramework/PlayerController.h"

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
			void Host(FString ServerName) override;

		UFUNCTION(Exec)
			void Join(uint32 Index) override;

		UPROPERTY(BlueprintReadOnly, Category = "PlayerInfo")
			int32 MaxPlayers = 4; // 최대 플레이어 수

		UPROPERTY(BlueprintReadOnly, Category = "PlayerInfo")
			TArray<int32> PlayerIndexes; // 각 플레이어의 고유 번호를 저장하는 배열

		UFUNCTION(BlueprintCallable, Category = "PlayerInfo")
			int32 GetPlayerIndex(APlayerController* PlayerController) const;

		UFUNCTION(BlueprintCallable, Category = "PlayerInfo")
			void RemovePlayerIndex(int32 PlayerIndex);

		void StartSession();

		virtual void LoadMainMenu() override;
		virtual void RefreshServerList() override;

		float PlayerIndex;
private:
	TSubclassOf<class UUserWidget>MenuClass; 
	TSubclassOf<class UUserWidget>InGameMenuClass; 


	IOnlineSessionPtr SessionInterface;
	TSharedPtr<class FOnlineSessionSearch> SessionSearch;

	void OnCreateSessionComplete(FName SessionName, bool succesess);
	void OnDestroySessionComplete(FName SessionName, bool succesess);
	void OnFindSessionsComplete(bool succesess);
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
	void OnNetworkFailure(UWorld* World, UNetDriver* NetDriver, ENetworkFailure::Type FailureType, const FString& ErrorString);


	FString DesiredServerName;
	void CreateSession();


	class UMainManu* Menu;

};
