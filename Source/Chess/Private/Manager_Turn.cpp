// Fill out your copyright notice in the Description page of Project Settings.

#include "Manager_Turn.h"

#include "Chess_GameMode.h"
#include "Chess_PlayerController.h"
#include "Blueprint/WidgetTree.h"
#include "Components/Button.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AManager_Turn::AManager_Turn()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Checker = nullptr;

	MovedPiece = nullptr;
	KilledPiece = nullptr;
	
	bIsKill = false;
	bIsPromotion = false;
	bIsBlackKingInCheck = false;
	bIsWhiteKingInCheck = false;

	PreviousTile = nullptr;
	NextTile = nullptr;
}

void AManager_Turn::SelfDestroy()
{
	Destroy();
}

void AManager_Turn::ResetVariables()
{
	Checker = nullptr;
	
	bIsKill = false;
	bIsPromotion = false;
	bIsBlackKingInCheck = false;
	bIsWhiteKingInCheck = false;

	PreviousTile = nullptr;
	NextTile = nullptr;
}

void AManager_Turn::SetTilesAndPieces(ATile* PTile, ATile* NTile, AChess_Piece* PieceToMove, AChess_Piece* PieceToKill)
{
	PreviousTile = PTile;
	NextTile = NTile;
	MovedPiece = PieceToMove;
	KilledPiece = PieceToKill;
}

void AManager_Turn::DisplayMove()
{
	const AChess_GameMode* GameMode = Cast<AChess_GameMode>(GetWorld()->GetAuthGameMode());
	AChess_PlayerController* Cpc = Cast<AChess_PlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	// Creazione del widget del pulsante
	UUserWidget* ButtonWidget = Cpc->UserInterfaceWidget->WidgetTree->ConstructWidget<UUserWidget>(MhButtonClass);
	FMoveInfo Move = {ButtonWidget, MovedPiece, KilledPiece, PreviousTile, NextTile};
	MoveHistory.Add(Move);
	CurrentButtonIndex = GameMode->MoveCounter - 1;
	// GameMode->MhButtons.Add(Move);
	
	
	// If it is the Human
	if (!GameMode->CurrentPlayer)
	{
		UVerticalBox* Container = Cast<UVerticalBox>(Cpc->UserInterfaceWidget->WidgetTree->FindWidget("MH_White"));
		
		if (Container)
		{
			Container->AddChild(ButtonWidget);
		}
	}
	else
	{
		UVerticalBox* Container = Cast<UVerticalBox>(Cpc->UserInterfaceWidget->WidgetTree->FindWidget("MH_Red"));

		if (Container)
		{
			Container->AddChild(ButtonWidget);
		}
	}
}

void AManager_Turn::DisplayEndGame() const
{
	AChess_PlayerController* Cpc = Cast<AChess_PlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	UScrollBox* Container = Cast<UScrollBox>(Cpc->UserInterfaceWidget->WidgetTree->FindWidget("MH_Scroll"));

	if (Container)
	{
		UUserWidget* LastButton = Cpc->UserInterfaceWidget->WidgetTree->ConstructWidget<UUserWidget>(EndButtonClass);
		LastButton->Rename(*FString::Printf(TEXT("EndGame")));
		Container->AddChild(LastButton);
	}
}

FString AManager_Turn::ComputeNotation() const
{
	const AChess_GameMode* GameMode = Cast<AChess_GameMode>(GetWorld()->GetAuthGameMode());
	const AManager_Turn* TurnManager = GameMode->TurnManager;
	if (!(NextTile || MovedPiece))
	{
		return ""; // Gestione errore
	}

	FString MoveNomenclature = "";
	if (TurnManager->bIsPromotion)
	{
		MoveNomenclature = MoveNomenclature + FString::Printf(TEXT("%c/%c%d"), MovedPiece->GetNomenclature(), NextTile->GetAlgebraicPosition().TileLetter, NextTile->GetAlgebraicPosition().TileNumber);
	}
	else if (MovedPiece->GetType() == EPieceType::PAWN)
	{
		if (PreviousTile && TurnManager->bIsKill)
		{
			MoveNomenclature = FString::Printf(TEXT("%cx"), PreviousTile->GetAlgebraicPosition().TileLetter);
		}
		MoveNomenclature = MoveNomenclature + FString::Printf(TEXT("%c%d"), NextTile->GetAlgebraicPosition().TileLetter, NextTile->GetAlgebraicPosition().TileNumber);
	}
	else
	{
		MoveNomenclature = FString::Printf(TEXT("%c"), MovedPiece->GetNomenclature());
		if (TurnManager->bIsKill)
		{
			MoveNomenclature = MoveNomenclature + "x";
		}
		MoveNomenclature = MoveNomenclature + FString::Printf(TEXT("%c%d"), NextTile->GetAlgebraicPosition().TileLetter, NextTile->GetAlgebraicPosition().TileNumber);
	}

	if (GameMode->bIsWhiteKingInCheckMate || GameMode->bIsBlackKingInCheckMate)
	{
		MoveNomenclature = MoveNomenclature + "#";
	}
	else if (TurnManager->bIsWhiteKingInCheck || TurnManager->bIsBlackKingInCheck)
	{
		MoveNomenclature = MoveNomenclature + "+";
	} 
	return MoveNomenclature;
}

void AManager_Turn::DestroyMoveHistory() const
{
	AChess_PlayerController* Cpc = Cast<AChess_PlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	UVerticalBox* WhiteHistory = Cast<UVerticalBox>(Cpc->UserInterfaceWidget->WidgetTree->FindWidget("MH_White"));
	UVerticalBox* RedHistory = Cast<UVerticalBox>(Cpc->UserInterfaceWidget->WidgetTree->FindWidget("MH_Red"));
	UUserWidget* EndButton = Cast<UUserWidget>(Cpc->UserInterfaceWidget->WidgetTree->FindWidget("EndGame"));

	if (WhiteHistory && RedHistory)
	{
		WhiteHistory->ClearChildren();
		RedHistory->ClearChildren();
	}
	if (EndButton)
	{
		UScrollBox* ScrollBox = Cast<UScrollBox>(Cpc->UserInterfaceWidget->WidgetTree->FindWidget("MH_Scroll"));
		ScrollBox->RemoveChild(EndButton);
	}
}

void AManager_Turn::Replay(const int32 ClickedIndex)
{
	// int32 i = MoveHistory.Num() - 1;
	// UScrollBox* MhContainer = Cast<UScrollBox>(WidgetTree->FindWidget("MH_Scroll"));
	AChess_GameMode* GameMode = Cast<AChess_GameMode>(GetWorld()->GetAuthGameMode());
	if (ClickedIndex >= 0 && ClickedIndex < CurrentButtonIndex)
	{
		int32 i = CurrentButtonIndex;
		while (i > 0 && i > ClickedIndex)
		{
			// Ripristina il pezzo mangiato
			if (MoveHistory[i].KilledPiece)
			{
				MoveHistory[i].KilledPiece->SetActorEnableCollision(true);
				MoveHistory[i].KilledPiece->SetActorHiddenInGame(false);
				MoveHistory[i].NextTile->SetPieceOnTile(MoveHistory[i].KilledPiece);
				MoveHistory[i].NextTile->SetTileStatus(ETileStatus::OCCUPIED);
				MoveHistory[i].NextTile->SetTileTeam(MoveHistory[i].KilledPiece->GetTeam());
				if (KilledPiece->GetTeam() == WHITE)
				{
					GameMode->WhiteTeam.Add(KilledPiece);
				}
				else
				{
					GameMode->BlackTeam.Add(KilledPiece);
				}
			}
			MoveHistory[i].MovedPiece->MovePiece(MoveHistory[i].PreviousTile);
			// MoveHistory[i].Button->RemoveFromParent();
			GameMode->UpdateScores();
			GameMode->MoveCounter - 1;
			i--;
		}
	}
	if (ClickedIndex <= MoveHistory.Num() - 1 && ClickedIndex > CurrentButtonIndex)
	{
		int32 i = CurrentButtonIndex;
		while (i <= MoveHistory.Num() - 1 && i <= ClickedIndex)
		{
			// Mangia il pezzo
			if (MoveHistory[i].KilledPiece)
			{
				if (MoveHistory[i].KilledPiece->GetTeam() == ETeam::WHITE)
				{
					GameMode->WhiteTeam.Remove(MoveHistory[i].KilledPiece);
					GameMode->KilledWhiteTeam.Add(MoveHistory[i].KilledPiece);
				}
				else
				{
					GameMode->BlackTeam.Remove(MoveHistory[i].KilledPiece);
					GameMode->KilledBlackTeam.Add(MoveHistory[i].KilledPiece);
				}
				MoveHistory[i].KilledPiece->SetActorHiddenInGame(true);
				MoveHistory[i].KilledPiece->SetActorEnableCollision(false);
			}
			MoveHistory[i].MovedPiece->MovePiece(MoveHistory[i].NextTile);
			// MoveHistory[i].Button->RemoveFromParent();
			GameMode->UpdateScores();
			GameMode->MoveCounter + 1;
			i++;
		}
	}
	CurrentButtonIndex = ClickedIndex;
}

// Called when the game starts or when spawned
void AManager_Turn::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AManager_Turn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

