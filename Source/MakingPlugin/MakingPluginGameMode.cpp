// Copyright Epic Games, Inc. All Rights Reserved.

#include "MakingPluginGameMode.h"
#include "MakingPluginCharacter.h"
#include "UObject/ConstructorHelpers.h"

AMakingPluginGameMode::AMakingPluginGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
