// Copyright Epic Games, Inc. All Rights Reserved.

#include "MCBlockGrid.h"

#include "Components/TextRenderComponent.h"
#include "Engine/World.h"
#include "MCGameInstance.h"
#include "MCGameMode.h"
#include "MCPlayerController.h"
#include "Kismet/GameplayStatics.h" 
#define LOCTEXT_NAMESPACE "PuzzleBlockGrid"

AMCBlockGrid::AMCBlockGrid()
//初始化列表
	:MinesSpacing(50.f)
	, Zoom(0.18)
	, CheckSoundVolume(1)
	, BoomSoundVolume(1)
{
	// Create dummy root scene component
	DummyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Dummy0"));
	RootComponent = DummyRoot;

	//加载声音
	static ConstructorHelpers::FObjectFinder<USoundWave> CheckSound(TEXT("SoundWave'/Game/MineClearance/Audio/UI/MineClick.MineClick'"));
	CheckSoundWave = CheckSound.Object;

	static ConstructorHelpers::FObjectFinder<USoundWave> BoomSound(TEXT("SoundWave'/Game/MineClearance/Audio/UI/Explode.Explode'"));
	BoomSoundWave = BoomSound.Object;
}


AMCBlock* AMCBlockGrid::GetMinesArray(const int X, const int Y)
{
	if (X>=0 && X < MinesWidth && Y >=0 && Y <MinesHeight)
	{
		 return MinesArray[Y * MinesWidth + X];
	}
	return nullptr;
	
}





void AMCBlockGrid::BeginPlay()
{
	Super::BeginPlay();

	if (GetGameInstance()) {
		UMCGameInstance* GI = Cast<UMCGameInstance>(GetGameInstance());
		MinesWidth = GI->MinesWidth;
		MinesHeight = GI->MinesHeight;
		MinesNumber = GI->MinesNumber;
		BlockNumber	= (MinesWidth * MinesHeight);
		RandTimes = BlockNumber;
		ShowedMinesNumber = (MinesWidth * MinesHeight);
		CurrentFlagNum = MinesNumber;
	}

	//生成地雷
	int num = 0;
	for (int y = 0; y < MinesHeight; y++)
	{
		for (int x = 0; x < MinesWidth; x++)
		{
			const float XOffset = (x - MinesWidth / 2.f) * MinesSpacing;
			const float YOffset = (y - MinesHeight / 2.f) * MinesSpacing;
			const FVector MineLocation = FVector(XOffset, YOffset, 10.f) + GetActorLocation();

			AMCBlock* NewMine = GetWorld()->SpawnActor<AMCBlock>(MineLocation, FRotator(0, 0, 0));

			if (NewMine != nullptr)
			{
				NewMine->OwningGrid = this;
				NewMine->SetActorRelativeScale3D(FVector(Zoom, Zoom, Zoom));
				NewMine->XOffset = x;
				NewMine->YOffset = y;
				MinesArray.Add(NewMine);
				//顺序布雷
				if (num < MinesNumber)
				{
					NewMine->MineInfo = -1;
					num++;
				}
			}
			else
			{
				x--;
			}
		
		}
	}

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::FromInt(MinesArray.Num()));
	//打乱地雷
	for (int i = 0; i < MinesNumber; i++)	 //有多少方块打乱多少次
	{
		const int Rand1 = FMath::Rand() % MinesNumber;//在这些里面随机选择
		const int Rand2 = MinesNumber + FMath::Rand() % (BlockNumber - MinesNumber);

		if (Rand1 == Rand2 || MinesArray[Rand1]->MineInfo == MinesArray[Rand2]->MineInfo)
			continue;
		const int TempMineInfo = MinesArray[Rand1]->MineInfo;
		MinesArray[Rand1]->MineInfo = MinesArray[Rand2]->MineInfo;
		MinesArray[Rand2]->MineInfo = TempMineInfo;

	}
	//二次均匀打乱
	for (int i = 0; i < RandTimes; i++)	 //有多少方块打乱多少次
	{
		const int Rand1 = FMath::Rand() % BlockNumber;//在这些里面随机选择
		const int Rand2 = FMath::Rand() % BlockNumber;

		if (Rand1 == Rand2 || MinesArray[Rand1]->MineInfo == MinesArray[Rand2]->MineInfo)
			continue;
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::FromInt(Rand1));
		const int TempMineInfo = MinesArray[Rand1]->MineInfo;
		MinesArray[Rand1]->MineInfo = MinesArray[Rand2]->MineInfo;
		MinesArray[Rand2]->MineInfo = TempMineInfo;

	}

	//计算周围的地雷数量
	  //对所有地雷进行周围检测
	for (int y = 0; y < MinesHeight; y++)
	{
		for (int x = 0; x < MinesWidth; x++)
		{
			//如果是地雷就跳过
			if (GetMinesArray(x,y)->MineInfo == -1)
				continue;
			//如果不是雷，即空白则计算周围的雷数量
			InitAroundMine(x, y);
		}
	}



}

//检测空白区域周围的地雷数量
void AMCBlockGrid::InitAroundMine(const int X, const int Y)
{
	if (X >= 0 && X < MinesWidth && Y >= 0 && Y < MinesHeight)
	{
		if (GetMinesArray(X,Y)->MineInfo != -1)//如果不是地雷
		{
			for (int y = -1; y <= 1; y++)
			{
				for (int x = -1; x <= 1; x++)
				{
					//在没地雷的地方，检测周围的地雷数量
					//检测周围位置合格以后检测地雷
					if (X + x >= 0 && X + x < MinesWidth && Y + y >= 0 && Y + y < MinesHeight && GetMinesArray(X + x, Y + y)->MineInfo == -1) 
					{
						GetMinesArray(X,Y)->MineInfo++;
					}
				}
			}
		}
	}
}

/*
	递归检测周围，
*/
void AMCBlockGrid::CheckAround(const int X, const int Y, const bool bNextToParent)
{
	//素质检测传进来的X,Y是否在当前矩阵范围内
	if (X >= 0 && X < MinesWidth && Y >= 0 && Y < MinesHeight)
	{
		//当这个方块未显示或标记为问号时可以进行检测
		if (GetMinesArray(X,Y)->ControlInfo ==0 || GetMinesArray(X, Y)->ControlInfo == 2)
		{
			//当检测的位置为一个1-8的数字
			if(GetMinesArray(X, Y) ->MineInfo >= 1 && GetMinesArray(X, Y)->MineInfo <= 8)
			{
				
				//当前检测的Block->MCBlockShowText(A[y][x]);
				GetMinesArray(X, Y)->SetNumText();//直接显示数字
				GetMinesArray(X, Y)->ControlInfo = 3;//标记已经操作过
				GetMinesArray(X, Y)->GetBlockMesh()->SetVisibility(false);//方块隐藏

				ShowedMinesNumber--;//用于计算当前已经展开的所有方块
			}
			else if (GetMinesArray(X, Y)->MineInfo == 0)//能走到这里说明当前方块已经是空的，只有空的方块才可以对其周围进行递归检查
			{
				//如果当前检测的节点紧挨着扩展它的父级节点
				//即当前节点在其父级节点的上、下、左或右
				if (bNextToParent)
				{
					GetMinesArray(X, Y)->ControlInfo = 3;//标记方块已经操作过
					GetMinesArray(X, Y)->GetBlockMesh()->SetVisibility(false);//方块隐藏
					ShowedMinesNumber--;//当前已经展开的节点数量
				
					/*递归检测当前方块的周围8个方块*/
					
					//上下左右
					CheckAround(X - 1, Y);
					CheckAround(X + 1, Y);
					CheckAround(X, Y - 1);
					CheckAround(X, Y + 1);

					//上左，上右，下左，下右四个方块
					//第三个参数默认false
					CheckAround(X - 1, Y - 1, false);
					CheckAround(X - 1, Y + 1, false);
					CheckAround(X + 1, Y - 1, false);
					CheckAround(X + 1, Y + 1, false);
				}
			}
		}
	}
}



void AMCBlockGrid::GameWin()
{
	if (ShowedMinesNumber == MinesNumber) //仅当剩余的全是雷时才成功
	{
		for (int y = 0; y < MinesHeight; y++)
		{
			for (int x = 0; x < MinesWidth; x++)
			{
				if (GetMinesArray(x, y)->MineInfo == -1)
				{
					GetMinesArray(x, y)->ControlInfo = 3;
					GetMinesArray(x, y)->GetMineMesh()->SetVisibility(true);//隐藏模型
					GetMinesArray(x, y)->GetBlockMesh()->SetVisibility(false);//隐藏模型
				}
			}
		}
		AMCPlayerController* CurrentPlayerController = Cast<AMCPlayerController>(UGameplayStatics::GetPlayerController(GWorld, 0));
		CurrentPlayerController->GameResult = 1;
	}
}

void AMCBlockGrid::GameFailed()
{
	for (int y = 0; y < MinesHeight; y++)
	{
		for (int x = 0; x < MinesWidth; x++)
		{
			if (GetMinesArray(x, y)->MineInfo == -1)
			{
				GetMinesArray(x, y)->ControlInfo = 3;
				GetMinesArray(x, y)->GetMineMesh()->SetVisibility(true);//隐藏模型
				GetMinesArray(x, y)->GetBlockMesh()->SetVisibility(false);//隐藏模型
			}
		}
	}
	AMCPlayerController* CurrentPlayerController = Cast<AMCPlayerController>(UGameplayStatics::GetPlayerController(GWorld,0));
	CurrentPlayerController->GameResult = 2;
}

void AMCBlockGrid::CheckSound()
{
	UGameplayStatics::PlaySound2D(GetWorld(), CheckSoundWave, CheckSoundVolume);
}


void AMCBlockGrid::BoomSound()
{
	UGameplayStatics::PlaySound2D(GetWorld(), BoomSoundWave, BoomSoundVolume);
}


#undef LOCTEXT_NAMESPACE
