// Fill out your copyright notice in the Description page of Project Settings.


#include "MCGameInstance.h"

UMCGameInstance::UMCGameInstance()
	:PlayerName("Player")
	, MinesWidth(9)
	, MinesHeight(9)
	, MinesNumber(10)
	, bFirstPlayer(false)
{
}

void UMCGameInstance::SetMinesInfo(const FString NewPlayerName, const int NewMinesWidth, const int NewMinesHeight, const int NewMinesNumber,const bool bNewFirstPlayer)
{
	PlayerName = NewPlayerName;
	MinesWidth = NewMinesWidth;
	MinesHeight = NewMinesHeight;
	MinesNumber = NewMinesNumber;
	bFirstPlayer = bNewFirstPlayer;
}
