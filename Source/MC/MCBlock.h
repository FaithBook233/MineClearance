// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MCBlock.generated.h"

/** A block that can be clicked */
UCLASS(minimalapi)
class AMCBlock : public AActor
{
	GENERATED_BODY()

	/** Dummy root component */
	UPROPERTY(Category = Block, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* DummyRoot;

	/** StaticMesh component for the clickable block */
	UPROPERTY(Category = Block, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* BlockMesh;

	UPROPERTY(Category = Block, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* FlagMesh;

	UPROPERTY(Category = Block, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* MineMesh;

public:
	AMCBlock();


	/** Pointer to white material used on the focused block */
	UPROPERTY()
	class UMaterial* BaseMaterial;

	//UPROPERTY()
	//class UMaterial* FlagMaterial;

	/** Pointer to blue material used on inactive blocks */
	UPROPERTY()
	class UMaterialInstance* BlueMaterial;

	/** Pointer to orange material used on active blocks */
	UPROPERTY()
	class UMaterialInstance* OrangeMaterial; 
	
	UPROPERTY()
	class UMaterialInstance* RedMaterial;
	
	UPROPERTY()
	class UMaterialInstance* GreenMaterial;

	/** Grid that owns us */
	UPROPERTY(Category = "MineInfo", EditAnywhere, BlueprintReadWrite)
	class AMCBlockGrid* OwningGrid;

	/** Handle the block being clicked */
	UFUNCTION()
	void BlockClicked(UPrimitiveComponent* ClickedComp, FKey ButtonClicked);

	/** Handle the block being touched  */
	UFUNCTION(BlueprintCallable, Category = "MineBlock")
	void OnFingerPressedBlock(ETouchIndex::Type FingerIndex, UPrimitiveComponent* TouchedComponent);
	
	UFUNCTION(BlueprintCallable, Category = "MineBlock")
	void HandleClicked();

	UFUNCTION(BlueprintCallable, Category = "MineBlock")
	void Highlight(bool bOn);


public:
	/** Returns DummyRoot subobject **/
	FORCEINLINE class USceneComponent* GetDummyRoot() const { return DummyRoot; }
	/** Returns BlockMesh subobject **/
	FORCEINLINE class UStaticMeshComponent* GetBlockMesh() const { return BlockMesh; }

	FORCEINLINE class UStaticMeshComponent* GetMineMesh() const { return MineMesh; }
	
	FORCEINLINE class UStaticMeshComponent* GetFlagMesh() const { return FlagMesh; }

public:
	//以下为自己加的变量
	UPROPERTY(Category = "MineInfo", EditAnywhere, BlueprintReadWrite)
	int MineInfo;

	UPROPERTY(Category = "MineInfo", EditAnywhere, BlueprintReadWrite)
	int ControlInfo;

	UPROPERTY(Category = "MineInfo", EditAnywhere, BlueprintReadWrite)
	FVector TextLocation;

	UPROPERTY(Category = "MineInfo", EditAnywhere, BlueprintReadWrite)
	FRotator TextRotation;

	UPROPERTY(Category = "MineInfo", EditAnywhere, BlueprintReadWrite)
	int XOffset;

	UPROPERTY(Category = "MineInfo", EditAnywhere, BlueprintReadWrite)
	int YOffset;

	UPROPERTY(Category = Grid, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UTextRenderComponent* NumText;

	UFUNCTION(BlueprintCallable, Category = "MineBlock")
	void SetNumText();

	UFUNCTION(BlueprintCallable, Category = "MineBlock")
	void SwitchSign();

};



