// Fill out your copyright notice in the Description page of Project Settings.

#include "MainManu.h"
#include "UObject/ConstructorHelpers.h"

#include "Components/Button.h" 
#include "Components/WidgetSwitcher.h" 
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"
#include "ServerRow.h"


UMainManu::UMainManu(const FObjectInitializer& FObjectInitializer)
{
	ConstructorHelpers::FClassFinder<UUserWidget> ServerRowBPClass(TEXT("/Game/menusystem/WBP_ServerRow"));
	if (!ensure(ServerRowBPClass.Class != nullptr)) return;

	ServerRowClass = ServerRowBPClass.Class;
}

bool UMainManu::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) return false;


	HostButton->OnClicked.AddDynamic(this, &UMainManu::OpenHoseMenu);

	ConfirmHostButton->OnClicked.AddDynamic(this, &UMainManu::HostServer);

	CancelHostButton->OnClicked.AddDynamic(this, &UMainManu::OpenMainMenu);

	JoinButton->OnClicked.AddDynamic(this, &UMainManu::OpenJoinServer);

	ConfirmJoinButton_1->OnClicked.AddDynamic(this, &UMainManu::JoinServer);

	ConfirmQeitButton->OnClicked.AddDynamic(this, &UMainManu::OpenMainMenu);


	Reset->OnClicked.AddDynamic(this, &UMainManu::OpenJoinServer);
	return true;
}

void UMainManu::HostServer()
{
	if (MainMenuInterface != nullptr)
	{
		FString ServerName = ServerHostName->Text.ToString();
		MainMenuInterface->Host(ServerName);
	}
}

void UMainManu::SelectIndex(uint32 Index)
{
	selectedIndex = Index;
	UpdateChildren();
}

void UMainManu::UpdateChildren()
{
	for (int32 i = 0; i < ServerList->GetChildrenCount(); ++i)
	{
		auto Row = Cast<UServerRow>(ServerList->GetChildAt(i));
		if (Row != nullptr)
		{
			Row->Selected = (selectedIndex.IsSet() && selectedIndex.GetValue() == i);
		}
	}
}
void UMainManu::JoinServer()
{
	if (selectedIndex.IsSet() && MainMenuInterface != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Selected index %d"), selectedIndex.GetValue());
		MainMenuInterface->Join(selectedIndex.GetValue());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Selected index Not Set"));
	}
}

void UMainManu::ResetServer()
{
	ManuSwitcher->SetActiveWidget(JoinManu);

	if (MainMenuInterface != nullptr)
		MainMenuInterface->RefreshServerList();
}

void UMainManu::OpenJoinServer()
{
	ManuSwitcher->SetActiveWidget(JoinManu);

	if (MainMenuInterface != nullptr)
		MainMenuInterface->RefreshServerList();
}

void UMainManu::SetServerList(TArray<FServerData> servername)
{

	UWorld* World = this->GetWorld();
	if (!ensure(World != nullptr)) return;

	ServerList->ClearChildren();

	uint32 i = 0;
	for (const FServerData& ServerData : servername)
	{
		UServerRow* Row = CreateWidget<UServerRow>(World, ServerRowClass);
		if (!ensure(Row != nullptr)) return;

		Row->ServerName->SetText(FText::FromString(ServerData.Name));
		Row->HostUser->SetText(FText::FromString(ServerData.HostUsername));
		FString FractionText = FString::Printf(TEXT("%d/%d"), ServerData.CurrentPlayers, ServerData.MaxPlayer);
		Row->ConnectionFraction->SetText(FText::FromString(FractionText));
		Row->SetUp(this, i);
		++i;

		ServerList->AddChild(Row);
	}
}


void UMainManu::OpenMainMenu()
{
	ManuSwitcher->SetActiveWidget(ManuSwitcher);
	if (MainMenuInterface != nullptr) {
		MainMenuInterface->RefreshServerList();
	}
}


void UMainManu::OpenHoseMenu()
{
	ManuSwitcher->SetActiveWidget(HostMenu);
}
