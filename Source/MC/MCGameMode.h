// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MCGameMode.generated.h"

/** GameMode class to specify pawn and playercontroller */
UCLASS(minimalapi)
class AMCGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AMCGameMode();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Info")
	int GameResultSign;
};



