// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once
#include "MCBlock.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Containers/Array.h" 
#include "MCBlockGrid.generated.h"

UCLASS(minimalapi)
class AMCBlockGrid : public AActor
{
	GENERATED_BODY()

	/** Dummy root component */
	UPROPERTY(Category = Grid, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* DummyRoot;


public:
	AMCBlockGrid();

public:
	//这些都是自己的新加变量
	UPROPERTY(Category = "MinesInfo", EditAnywhere, BlueprintReadWrite)
	float MinesSpacing;

	UPROPERTY(Category = "MinesInfo", EditAnywhere, BlueprintReadWrite)
	int MinesWidth;

	UPROPERTY(Category = "MinesInfo", EditAnywhere, BlueprintReadWrite)
	int MinesHeight;
	
	UPROPERTY(Category = "MinesInfo", EditAnywhere, BlueprintReadWrite)
	int MinesNumber;
	
	UPROPERTY(Category = "MinesInfo", EditAnywhere, BlueprintReadWrite)
	int BlockNumber;

	UPROPERTY(Category = "MinesInfo", EditAnywhere, BlueprintReadWrite)
	float Zoom;

	UPROPERTY(Category = "MinesInfo", EditAnywhere, BlueprintReadWrite)
	int RandTimes;

	UPROPERTY(Category = "MinesInfo", EditAnywhere, BlueprintReadWrite)
	int ShowedMinesNumber;

	UPROPERTY(Category = "MinesInfo", EditAnywhere, BlueprintReadWrite)
	TArray<AMCBlock*>MinesArray;

	UPROPERTY(Category = "MinesInfo", EditAnywhere, BlueprintReadWrite)
	int CurrentFlagNum;

	UFUNCTION(BlueprintCallable, Category = "MCBlockGrid")
		AMCBlock* GetMinesArray(const int X, const int Y);

	UFUNCTION(BlueprintCallable, Category = "MCBlockGrid")
	void InitAroundMine(const int X,const int Y);

	UFUNCTION(BlueprintCallable, Category = "MCBlockGrid")
	void CheckAround(const int X, const int Y, const bool bNextToParent = true);

	UFUNCTION(BlueprintCallable, Category = "MCBlockGrid")
	void GameFailed();

	UFUNCTION(BlueprintCallable, Category = "MCBlockGrid")
	void GameWin();

	UFUNCTION(BlueprintCallable, Category = "MCBlockGrid")
	void CheckSound();

	UFUNCTION(BlueprintCallable, Category = "MCBlockGrid")
	void BoomSound();

	UPROPERTY(Category = "MinesInfo", EditAnywhere, BlueprintReadWrite)
	USoundWave* CheckSoundWave;
	
	UPROPERTY(Category = "MinesInfo", EditAnywhere, BlueprintReadWrite)
	USoundWave* BoomSoundWave;

	UPROPERTY(Category = "MinesInfo", EditAnywhere, BlueprintReadWrite)
	float CheckSoundVolume;
							   
	UPROPERTY(Category = "MinesInfo", EditAnywhere, BlueprintReadWrite)
	float BoomSoundVolume;

protected:
	// Begin AActor interface
	virtual void BeginPlay() override;

public:


	/** Returns DummyRoot subobject **/
	FORCEINLINE class USceneComponent* GetDummyRoot() const { return DummyRoot; }
	
};



