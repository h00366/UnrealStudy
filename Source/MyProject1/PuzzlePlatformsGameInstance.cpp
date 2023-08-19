// Fill out your copyright notice in the Description page of Project Settings.


#include "UObject/ConstructorHelpers.h"
#include "PuzzlePlatformsGameInstance.h"

#include "Engine/Engine.h"  
#include "Blueprint/UserWidget.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineSessionInterface.h"


#include "PlatformTrigger.h"
#include "MainManu.h"
#include "MainMenuInterface.h"
#include "MenuWidget.h"

const static FName SESSION_NAME = TEXT("My Session Gmae"); 

UPuzzlePlatformsGameInstance::UPuzzlePlatformsGameInstance(const FObjectInitializer& FObjectInitializer)
{
	ConstructorHelpers::FClassFinder<UUserWidget> MenuBPClass(TEXT("/Game/menusystem/WBP_MainManu"));
	if (!ensure(MenuBPClass.Class != nullptr)) return;

	MenuClass = MenuBPClass.Class;

	ConstructorHelpers::FClassFinder<UUserWidget> InGameMenuBPClass(TEXT("/Game/menusystem/WBP_IngameMenu"));
	if (!ensure(InGameMenuBPClass.Class != nullptr)) return;

	InGameMenuClass = InGameMenuBPClass.Class;

	UE_LOG(LogTemp, Warning, TEXT("GameInstance Constructor"));	

}

void UPuzzlePlatformsGameInstance::Init()
{
	UE_LOG(LogTemp, Warning, TEXT("GameInstance init"));
	{
		IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
		if (Subsystem != nullptr) {
			UE_LOG(LogTemp, Warning, TEXT("Found subsystem %s"), *Subsystem->GetSubsystemName().ToString());
			SessionInterface = Subsystem->GetSessionInterface();
			if (SessionInterface.IsValid()) {
				SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UPuzzlePlatformsGameInstance::OnCreateSessionComplete);
				SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UPuzzlePlatformsGameInstance::OnCreateSessionComplete);
				SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UPuzzlePlatformsGameInstance::OnFindSessionsComplete);
				SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UPuzzlePlatformsGameInstance::FOnJoinSessionComplete);

			}
		}
		else 
		{
			UE_LOG(LogTemp, Warning, TEXT("Found no subsystem"));
		}
	}
}


void UPuzzlePlatformsGameInstance::Join(uint32 Index)
{

	if (!SessionInterface.IsValid()) return;
	if (!SessionSearch.IsValid()) return;
	if (Menu != nullptr)
	{
		Menu->Teardown();
	}

	SessionInterface->JoinSession(0, SESSION_NAME, SessionSearch->SearchResults[Index]);
}
void UPuzzlePlatformsGameInstance::LoadMainMenu()
{
	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!ensure(PlayerController != nullptr)) return;
	PlayerController->ClientTravel("/Game/Map/MainMenu", ETravelType::TRAVEL_Absolute);

}

void UPuzzlePlatformsGameInstance::RefreshServerList()
{

	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	if (SessionSearch.IsValid())
	{
		//					SessionSearch->QuerySettings.Get
		SessionSearch->MaxSearchResults = 100;
		SessionSearch->QuerySettings.Set( SEARCH_PRESENCE, true,EOnlineComparisonOp::Equals);
		SessionSearch->bIsLanQuery = true;
		UE_LOG(LogTemp, Warning, TEXT("Starting FIne Session"));
		SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());

	}
}

void UPuzzlePlatformsGameInstance::Host()						// PlayerControllers, GameMode, Manager, Instances ����� ������
{
	if (SessionInterface.IsValid())
	{
		auto ExistingSession =  SessionInterface->GetNamedSession(SESSION_NAME);
		if (ExistingSession != nullptr)
		{

			SessionInterface->DestroySession(SESSION_NAME);
		}
		else
		{
			CreateSession();
		}
	}
}
void UPuzzlePlatformsGameInstance::OnCreateSessionComplete(FName SessionName, bool Success)
{
	if (!Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Could not create session"));
		return;
	}

	if (Menu != nullptr)
	{
		Menu->Teardown();
	}

	UEngine* Engine = GetEngine();
	if (!ensure(Engine != nullptr)) return;

	Engine->AddOnScreenDebugMessage(0, 2, FColor::Green, TEXT("Hosting"));

	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;

	World->ServerTravel("/Game/Map/ThirdPersonExampleMaps?listen");
}

void UPuzzlePlatformsGameInstance::OnDestroySessionComplete(FName SessionName, bool succesess)
{
	if (succesess)
	{
		CreateSession();
	}
}

void UPuzzlePlatformsGameInstance::OnFindSessionsComplete(bool succesess)
{
	if (succesess && SessionSearch.IsValid() && Menu != nullptr)
	{
		
		UE_LOG(LogTemp, Warning, TEXT("Finished Fine Session"));
		TArray<FString> Servernames;

		for (const FOnlineSessionSearchResult& SearchResult : SessionSearch->SearchResults)
		{
			UE_LOG(LogTemp, Warning, TEXT("Session name %s"), *SearchResult.GetSessionIdStr());
			Servernames.Add(SearchResult.GetSessionIdStr());
		}
		Menu->SetServerList(Servernames);

	}
}

void UPuzzlePlatformsGameInstance::CreateSession()
{
	if (SessionInterface.IsValid())
	{

	FOnlineSessionSettings SessionSettings;
	SessionSettings.bIsLANMatch = false;
	SessionSettings.NumPublicConnections = 2;
	SessionSettings.bShouldAdvertise = true;
	SessionSettings.bUsesPresence = true;

	SessionInterface->CreateSession(0, SESSION_NAME, SessionSettings);
	}

}


void UPuzzlePlatformsGameInstance::LoadMenu()
{
	if (!ensure(MenuClass != nullptr)) return;

	 Menu = CreateWidget<UMainManu>(this, MenuClass);

	if (!ensure(Menu != nullptr)) return;

	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!ensure(PlayerController != nullptr)) return;

	Menu->SetUp();
	Menu->SetMenuInterface(this); 
}

void UPuzzlePlatformsGameInstance::InGameLoadMenu()
{
	if (!ensure(InGameMenuClass != nullptr))
	{
		return;
	}
	UMenuWidget*  Menus = CreateWidget<UMenuWidget>(this, InGameMenuClass);
	if (!ensure(Menus != nullptr))
	{
		return;
	}
	Menus->SetUp();
	Menus->SetMenuInterface(this);
}


void UPuzzlePlatformsGameInstance::FOnJoinSessionComplete(FName sessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if (!SessionInterface.IsValid()) return;

	FString Address; 
	if(SessionInterface->GetResolvedConnectString(sessionName, Address))
	{
		UE_LOG(LogTemp, Warning, TEXT("Could not Get connect string"));
		return;
	}

	UEngine* Engine = GetEngine();
	if (!ensure(Engine != nullptr)) return;

	Engine->AddOnScreenDebugMessage(0, 2, FColor::Green, FString::Printf(TEXT("Joining %s"), *Address));

	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!ensure(PlayerController != nullptr)) return;
	PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
	}