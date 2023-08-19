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

void UMainManu::SetServerList(TArray<FString> servername)
{

	UWorld* world = this->GetWorld();

	ServerList->ClearChildren();

	uint32 i = 0;

	for (const FString& Servername : servername)
	{
		UServerRow* Row = CreateWidget<UServerRow>(world, ServerRowClass);

		Row->ServerName->SetText(FText::FromString(Servername));
		Row->SetUp(this,i );
		++i;
		ServerList->AddChild(Row);
	}
}


bool UMainManu::Initialize()
 {
	bool Success = Super::Initialize();
	if (!Success) return false;  


	HostButton->OnClicked.AddDynamic(this, &UMainManu::HostServer);

	JoinButton->OnClicked.AddDynamic(this, &UMainManu::OpenJoinServer);

	ConfirmJoinButton_1->OnClicked.AddDynamic(this, &UMainManu::JoinServer);

	ConfirmQeitButton->OnClicked.AddDynamic(this, &UMainManu::JoinServer);
	
	 	return true;
 }

void UMainManu::OpenJoinServer()
{
	ManuSwitcher->SetActiveWidget(JoinManu);

	if (MainMenuInterface != nullptr)
	{
		MainMenuInterface->RefreshServerList();
	}
}

void UMainManu::HostServer()
{
	if (MainMenuInterface != nullptr)
	{
		MainMenuInterface->Host();
	}

}
void UMainManu::SelectIndex(uint32 Index)
{
	selectedIndex = Index;
}

void UMainManu::JoinServer()
{
	if (selectedIndex.IsSet()&& MainMenuInterface != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Selected index %d"), selectedIndex.GetValue());
		MainMenuInterface->Join(selectedIndex.GetValue());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Selected index Not Set")); 
	}
}