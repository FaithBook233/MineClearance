// Copyright Epic Games, Inc. All Rights Reserved.

#include "MCBlock.h"
#include "MCBlockGrid.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Materials/MaterialInstance.h"
#include "Components/TextRenderComponent.h"
#include "Components\InputComponent.h"
#include "Kismet/GameplayStatics.h" 
#include "MCPlayerController.h"
AMCBlock::AMCBlock()
{
	// 变量初始化
	TextLocation = FVector(0.f, 0.f, 166.666656f);
	TextRotation = FRotator(90.f, 0.f, -90.f);
	MineInfo = 0;
	ControlInfo = 0;

	// Structure to hold one-time initialization
	struct FConstructorStatics
	{
		ConstructorHelpers::FObjectFinderOptional<UStaticMesh> PlaneMesh;
		ConstructorHelpers::FObjectFinderOptional<UStaticMesh> FlagOnMineMesh;
		ConstructorHelpers::FObjectFinderOptional<UStaticMesh> Boom;

		ConstructorHelpers::FObjectFinderOptional<UMaterial> BaseMaterial;
		ConstructorHelpers::FObjectFinderOptional<UMaterialInstance> BlueMaterial;
		ConstructorHelpers::FObjectFinderOptional<UMaterialInstance> OrangeMaterial;
		ConstructorHelpers::FObjectFinderOptional<UMaterialInstance> RedMaterial;
		ConstructorHelpers::FObjectFinderOptional<UMaterialInstance> GreenMaterial;
		


		FConstructorStatics()
			: PlaneMesh(TEXT("/Game/MineClearance/Meshes/MineMesh.MineMesh"))
			, FlagOnMineMesh(TEXT("/Game/MineClearance/Meshes/Flag/Flag.Flag"))
			, Boom(TEXT("/Game/MineClearance/Meshes/Mine/MineMesh.MineMesh"))

			, BaseMaterial(TEXT("/Game/MineClearance/Material/BaseMaterial.BaseMaterial"))
			, BlueMaterial(TEXT("/Game/MineClearance/Material/BlueMaterial.BlueMaterial"))
			, OrangeMaterial(TEXT("/Game/MineClearance/Material/OrangeMaterial.OrangeMaterial"))
			, RedMaterial(TEXT("/Game/MineClearance/Material/RedMaterial.RedMaterial"))
			, GreenMaterial(TEXT("/Game/MineClearance/Material/GreenMaterial.GreenMaterial"))
			
		{
		}
	};
	static FConstructorStatics ConstructorStatics;

	// Save a pointer to the orange material
	BaseMaterial = ConstructorStatics.BaseMaterial.Get();
	BlueMaterial = ConstructorStatics.BlueMaterial.Get();
	OrangeMaterial = ConstructorStatics.OrangeMaterial.Get();
	RedMaterial = ConstructorStatics.RedMaterial.Get();
	GreenMaterial = ConstructorStatics.GreenMaterial.Get();



	// Create dummy root scene component
	DummyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Dummy0"));
	RootComponent = DummyRoot;


	NumText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("ScoreText0"));
	NumText->SetRelativeLocation(TextLocation);
	NumText->SetRelativeRotation(TextRotation);
	NumText->SetText(FText::FromString(" "));
	NumText->SetWorldSize(200);
	NumText->SetHorizontalAlignment(EHTA_Center);
	NumText->SetVerticalAlignment(EVRTA_TextCenter);
	NumText->SetupAttachment(DummyRoot);


	// Create static mesh component
	BlockMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BlockMesh0"));
	BlockMesh->SetStaticMesh(ConstructorStatics.PlaneMesh.Get());
	BlockMesh->SetRelativeScale3D(FVector(1.f,1.f,1.f)); 
	BlockMesh->SetRelativeLocation(FVector(0.f,0.f,25.f));
	BlockMesh->SetMaterial(0, ConstructorStatics.BlueMaterial.Get());
	BlockMesh->SetupAttachment(DummyRoot);
	BlockMesh->OnClicked.AddDynamic(this, &AMCBlock::BlockClicked);
	BlockMesh->OnInputTouchBegin.AddDynamic(this, &AMCBlock::OnFingerPressedBlock);

	FlagMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FlagMesh0"));
	FlagMesh->SetStaticMesh(ConstructorStatics.FlagOnMineMesh.Get());
	FlagMesh->SetRelativeLocation(FVector(0.f,0.f,128.f));
	FlagMesh->SetRelativeRotation(FRotator(90.f, 90.f, 0.f));
	FlagMesh->SetRelativeScale3D(FVector(2.f, 2.f, 2.f));
	FlagMesh->SetupAttachment(BlockMesh);
	FlagMesh->SetVisibility(false);
	

	MineMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MineMesh0"));
	MineMesh->SetStaticMesh(ConstructorStatics.Boom.Get());
	MineMesh->SetRelativeLocation(FVector(0.f, 0.f, 70.f));
	MineMesh->SetRelativeRotation(FRotator(0.f, 0.f, 0.f));
	MineMesh->SetRelativeScale3D(FVector(15.f, 15.f, 15.f));
	MineMesh->SetVisibility(false);
	MineMesh->SetupAttachment(BlockMesh);
	

}

void AMCBlock::BlockClicked(UPrimitiveComponent* ClickedComp, FKey ButtonClicked)
{
	HandleClicked();
}


void AMCBlock::OnFingerPressedBlock(ETouchIndex::Type FingerIndex, UPrimitiveComponent* TouchedComponent)
{
	HandleClicked();
}



void AMCBlock::HandleClicked()
{

	//当这个方块未标记或未显示时
	if (ControlInfo == 0 || ControlInfo == 2)
	{
		if (MineInfo == -1)
		{
			//显示地雷
			OwningGrid->BoomSound();
			MineMesh->SetMaterial(0, RedMaterial);
			OwningGrid->GameFailed();
		}
		else
		{
			if (MineInfo == 0)//当前位0，周围没有地雷
			{
				OwningGrid->CheckSound();
			}
			OwningGrid->CheckAround(XOffset, YOffset);
			OwningGrid->GameWin();
		}
	}
}

//按钮悬停材质变换
void AMCBlock::Highlight(bool bOn)
{
	//如果是地雷，旗子，问号或者是已经显示的模块则不处理

	AMCPlayerController* AMPC = Cast<AMCPlayerController>(UGameplayStatics::GetPlayerController(GWorld,0));

	if (ControlInfo > 0 || AMPC->Stop)
		return;

	if (bOn)
	{
		BlockMesh->SetMaterial(0, BaseMaterial);
	}
	else
	{
		BlockMesh->SetMaterial(0, BlueMaterial);
	}
}

void AMCBlock::SetNumText()
{
	if (MineInfo != 0)
	{
		NumText->SetText(FText::FromString(FString::FromInt(MineInfo)));
	}
	else
	{
		NumText->SetText(FText::FromString(""));
	}
		
}

void AMCBlock::SwitchSign()
{
	switch (ControlInfo)
	{
	case 0:ControlInfo++; FlagMesh->SetVisibility(true); BlockMesh->SetMaterial(0, BlueMaterial); OwningGrid->CurrentFlagNum--;  break;
	case 1:ControlInfo++;  FlagMesh->SetVisibility(false); NumText->SetText(FText::FromString("?")); OwningGrid->CurrentFlagNum++;  break;
	case 2:ControlInfo=0; NumText->SetText(FText::FromString(" "));break;
	}
}
