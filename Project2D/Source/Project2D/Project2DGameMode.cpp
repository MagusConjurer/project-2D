// Copyright Epic Games, Inc. All Rights Reserved.

#include "Project2DGameMode.h"
#include "Project2DCharacter.h"
#include "UObject/ConstructorHelpers.h"

AProject2DGameMode::AProject2DGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/SideScrollerCPP/Blueprints/SideScrollerCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
