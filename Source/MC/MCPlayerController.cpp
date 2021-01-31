// Copyright Epic Games, Inc. All Rights Reserved.

#include "MCPlayerController.h"

AMCPlayerController::AMCPlayerController()
	:GameResult(0)
	,Stop(false)
{
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableTouchEvents = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;
}
