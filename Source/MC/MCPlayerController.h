// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MCPlayerController.generated.h"

/** PlayerController class used to enable cursor */
UCLASS()
class AMCPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AMCPlayerController();

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "GameInfo")
	int GameResult;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameInfo")
	bool Stop;
};


