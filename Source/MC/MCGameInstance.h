// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MCGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class MC_API UMCGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:

	UMCGameInstance();

	UPROPERTY(Category = "GameInfo", EditAnywhere, BlueprintReadWrite)
	int MinesWidth;

	UPROPERTY(Category = "GameInfo", EditAnywhere, BlueprintReadWrite)
	int MinesHeight;

	UPROPERTY(Category = "GameInfo", EditAnywhere, BlueprintReadWrite)
	int MinesNumber;

	UPROPERTY(Category = "GameInfo", EditAnywhere, BlueprintReadWrite)
	FString PlayerName;

	UPROPERTY(Category = "GameInfo", EditAnywhere, BlueprintReadWrite)
	bool bFirstPlayer;

	UFUNCTION(BlueprintCallable, Category = "MCGameInstance")
		void SetMinesInfo(const FString NewPlayerName,const int NewMinesWidth,const int NewMinesHeight,const int NewMinesNumber,const bool bNewFirstPlayer);

};
