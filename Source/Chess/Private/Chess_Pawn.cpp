// Fill out your copyright notice in the Description page of Project Settings.


#include "Chess_Pawn.h"

#include "Utility.h"
#include "Chess_GameMode.h"
#include "Manager_Promotion.h"
#include "Manager_Turn.h"

AChess_Pawn::AChess_Pawn()
{
	// Template function that creates a components
	PawnComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	PawnMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));

	// Every actor has a RootComponent that defines the transform in the World
	SetRootComponent(PawnComponent);
	PawnMeshComponent->SetupAttachment(PawnComponent);

	Nomenclature = 'P';
	PieceValue = 1;
	FirstMove = true;
}

TArray<ATile*> AChess_Pawn::GetPossibleMoves()
{
	Super::GetPossibleMoves();
	TArray<ATile*> PossibleMoves;
	
	const TCHAR CurrLetter = CurrentTile->GetAlgebraicPosition().TileLetter;
	const uint8 CurrNumber = CurrentTile->GetAlgebraicPosition().TileNumber;

	// For white pawns
	if (GetTeam() == WHITE)
	{
		// Advance
		if (FirstMove)
		{
			if (Utility::IsValidPosition(CurrLetter, CurrNumber + 1))
			{
				ATile* PossibleMove = GameMode->GetTileAtPosition(CurrLetter, CurrNumber + 1);
				if (PossibleMove->GetTileStatus() == ETileStatus::EMPTY)
				{
					PossibleMoves.Add(PossibleMove);
				if (Utility::IsValidPosition(CurrLetter, CurrNumber + 2))
				{
					PossibleMove = GameMode->GetTileAtPosition(CurrLetter, CurrNumber + 2);
					if (PossibleMove->GetTileStatus() == ETileStatus::EMPTY)
					{
						PossibleMoves.Add(PossibleMove);
					}
				}
			}
			}
		}
		else
		{
			if (Utility::IsValidPosition(CurrLetter, CurrNumber + 1))
			{
				ATile* PossibleMove = GameMode->GetTileAtPosition(CurrLetter, CurrNumber + 1);
				if (PossibleMove->GetTileStatus() == ETileStatus::EMPTY)
				{
					PossibleMoves.Add(PossibleMove);
				}
			}
		}

		// Kill
		if (Utility::IsValidPosition(CurrLetter + 1, CurrNumber + 1))
		{
			ATile* UpRight = GameMode->GetTileAtPosition(CurrLetter + 1, CurrNumber + 1);

			if (UpRight && UpRight->GetTileStatus() == ETileStatus::OCCUPIED && (UpRight->GetTileTeam() != PieceTeam))
			{
				PossibleMoves.Add(UpRight);
			}
		}

		if (Utility::IsValidPosition(CurrLetter - 1, CurrNumber + 1))
		{
			ATile* UpLeft = GameMode->GetTileAtPosition(CurrLetter - 1, CurrNumber + 1);

			if (UpLeft && UpLeft->GetTileStatus() == ETileStatus::OCCUPIED && (UpLeft->GetTileTeam() != PieceTeam))
			{
				PossibleMoves.Add(UpLeft);
			}
		}
	}
	// For black pawns
	else
	{
		// Recede
		if (FirstMove)
		{
			ATile* PossibleMove = GameMode->GetTileAtPosition(CurrLetter, CurrNumber - 1);
			if (PossibleMove->GetTileStatus() == ETileStatus::EMPTY)
			{
				PossibleMoves.Add(PossibleMove);
				PossibleMove = GameMode->GetTileAtPosition(CurrLetter, CurrNumber - 2);
				if (PossibleMove->GetTileStatus() == ETileStatus::EMPTY)
				{
					PossibleMoves.Add(PossibleMove);
				}
			}
		}
		else
		{
			ATile* PossibleMove = GameMode->GetTileAtPosition(CurrLetter, CurrNumber - 1);
			if (PossibleMove->GetTileStatus() == ETileStatus::EMPTY)
			{
				// PossibleMove->TargetTile();
				PossibleMoves.Add(PossibleMove);
			}
		}

		// Kill
		ATile* DownRight = GameMode->GetTileAtPosition(CurrLetter + 1, CurrNumber - 1);
		ATile* DownLeft = GameMode->GetTileAtPosition(CurrLetter - 1, CurrNumber - 1);
		
		if (DownRight && DownRight->GetTileStatus() == ETileStatus::OCCUPIED && (DownRight->GetTileTeam() != PieceTeam))
		{
			PossibleMoves.Add(DownRight);
		}

		if (DownLeft && DownLeft->GetTileStatus() == ETileStatus::OCCUPIED && (DownLeft->GetTileTeam() != PieceTeam))
		{
			PossibleMoves.Add(DownLeft);
		}
	}

	return PossibleMoves;
}

void AChess_Pawn::SetMaterial(const int32 Index)
{
	Super::SetMaterial(Index);
	
	PawnMeshComponent->SetMaterial(0, PawnMaterials[Index]);
}

// Override to implement the promotion and to change
// the FirstMove boolean managing Pawn moves
void AChess_Pawn::MovePiece(ATile* NextTile)
{
	Super::MovePiece(NextTile);

	FirstMove = false;

	if (!this->IsHidden())
	{
		if (NextTile->GetAlgebraicPosition().TileNumber == 8)
		{
			AManager_Turn* TurnManager = GameMode->TurnManager;
			AManager_Promotion* PromotionManager = GameMode->PromotionManager;
		
			// Implement promotion for Human player
			TurnManager->bIsPromotion = true;

			GameMode->WhiteTeam.Remove(this);

			PromotionManager->SetCurrentPawn(this);
			PromotionManager->StartPromotion(this);
		}
		else if (NextTile->GetAlgebraicPosition().TileNumber == 1)
		{
			AManager_Turn* TurnManager = GameMode->TurnManager;
			AManager_Promotion* PromotionManager = GameMode->PromotionManager;
		
			// Implement promotion for AI player
			TurnManager->bIsPromotion = true;

			GameMode->BlackTeam.Remove(this);

			PromotionManager->SetCurrentPawn(this);
		
			const int32 RandomNumber = FMath::RandRange(0, 3);
			PromotionManager->HandleButtonClicked(RandomNumber);
		}
	}
}

/*
void AChess_Pawn::StartPromotion()
{
	if (GameMode)
	{
		AChess_PlayerController* Cpc = Cast<AChess_PlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
		
		PromotionWidget = CreateWidget<UManager_PromotionHUD>(Cpc, PromotionWidgetClass);
		PromotionWidget->SetCurrentPawn(this);
		PromotionWidget->AddToViewport();
	}
}

void AChess_Pawn::HandleButtonClicked(const int32 SelectedPieceIndex)
{
	AChess_PlayerController* Cpc = Cast<AChess_PlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	AChess_Piece* NewPiece;
	
	switch (SelectedPieceIndex)
	{
		case 0:
			NewPiece = GameMode->CBoard->SpawnSinglePiece(CurrentTile, PieceTeam, ROOK);
			break;
		case 1:
			NewPiece = GameMode->CBoard->SpawnSinglePiece(CurrentTile, PieceTeam, QUEEN);
			break;
		case 2:
			NewPiece = GameMode->CBoard->SpawnSinglePiece(CurrentTile, PieceTeam, BISHOP);
			break;
		case 3:
			NewPiece = GameMode->CBoard->SpawnSinglePiece(CurrentTile, PieceTeam, KNIGHT);
			break;
		default:
			NewPiece = nullptr;
			break;
	}
	
	switch (PieceTeam)
	{
	case WHITE:
		GameMode->WhiteTeam.Remove(this);
		GameMode->WhiteTeam.Add(NewPiece);
		break;
	case BLACK:
		GameMode->BlackTeam.Remove(this);
		GameMode->BlackTeam.Add(NewPiece);
		break;
	default:
		break;
	}
	
	Cpc->UserInterfaceWidget->SetPieceToMove(NewPiece);
	GameMode->UpdateScores();
	this->Destroy();
	
	if (PromotionWidget)
	{
		PromotionWidget->RemoveFromParent();
		// Probably I can't destroy the widget directly
		// Hoping the garbage collector take care of it
		PromotionWidget = nullptr;
		// GameMode->SetGamePaused(false);
	}
}
*/

void AChess_Pawn::BeginPlay()
{
	Super::BeginPlay();
}