// Copyright Epic Games, Inc. All Rights Reserved.

#include "MCGameMode.h"
#include "MCPlayerController.h"
#include "MCPawn.h"

AMCGameMode::AMCGameMode()
	:GameResultSign(0)
{
	// no pawn by default
	DefaultPawnClass = AMCPawn::StaticClass();
	// use our own player controller class
	PlayerControllerClass = AMCPlayerController::StaticClass();


}
