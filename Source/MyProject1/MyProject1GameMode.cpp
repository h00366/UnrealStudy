// Copyright Epic Games, Inc. All Rights Reserved.

#include "MyProject1GameMode.h"
#include "MyProject1Character.h"
#include "UObject/ConstructorHelpers.h"

AMyProject1GameMode::AMyProject1GameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
