// Fill out your copyright notice in the Description page of Project Settings.

#include "Chessboard.h"

#include "Chess_GameMode.h"
#include "Chess_King.h"

// Sets default values
AChessboard::AChessboard()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	FieldSize = 8;
	TileSize = 120;
}

FVector AChessboard::GetTilePosition(const FHitResult& Hit) const
{
	return Cast<ATile>(Hit.GetActor())->GetTileLocation();
}

int32 AChessboard::GetFieldSize() const
{
	return FieldSize;
}

float AChessboard::GetTileSize() const
{
	return TileSize;
}

AChess_Piece* AChessboard::SpawnSinglePiece(ATile* CurrentTile, const ETeam Team, const EPieceType Type)
{
	const FVector SpawnLocation = CurrentTile->GetTileLocation() + FVector (0,0,0.1);
	const FRotator SpawnRotation = FRotator(0.0f, 90.0f, 0.0f);
	
	AChess_Piece* Piece = GetWorld()->SpawnActor<AChess_Piece>(PieceClass[Type], SpawnLocation, SpawnRotation);

	Piece->SetTeam(Team);
	Piece->SetType(Type);
	Piece->SetPieceTile(CurrentTile);
	Piece->SetActorScale3D(FVector(TileSize/100, TileSize/100, 0.05));
	Piece->SetMaterial(Team);
	Piece->GetPieceTile()->SetTileStatus(ETileStatus::OCCUPIED);
	Piece->GetPieceTile()->SetTileTeam(Piece->GetTeam());

	// Set the reference of the Piece above the Tile
	CurrentTile->SetPieceOnTile(Piece);

	return Piece;
}

// Called when the game starts or when spawned
void AChessboard::BeginPlay()
{
	Super::BeginPlay();

	// Get the reference to the Chess_GameMode
	AChess_GameMode* GameMode = Cast<AChess_GameMode>(GetWorld()->GetAuthGameMode());
	// Add SelfDestroy method to the broadcast OnResetEvent delegate
	GameMode->OnResetEvent.AddDynamic(this, &AChessboard::SelfDestroy); 

	// Generate the board and spawn each piece
	GenerateChessBoard();
	SpawnPieces();
}

/*
 * Generate the playing field
 */
void AChessboard::GenerateChessBoard() const
{
	// Get the reference to the Chess_GameMode
	AChess_GameMode* GameMode = Cast<AChess_GameMode>(GetWorld()->GetAuthGameMode());
	
	for (int x = 0; x < FieldSize; x++)
	{
		for (int y = 0; y < FieldSize; y++)
		{
			FVector SpawnLocation = FVector(x * TileSize, y * TileSize, 0);
			FRotator SpawnRotation = FRotator(0, 0, 0);
			
			ATile* TileObject = GetWorld()->SpawnActor<ATile>(TileClass, SpawnLocation, SpawnRotation);

			const float TileScale = TileSize / 100;
			
			TileObject->SetActorScale3D(FVector(TileScale, TileScale, 0.2));
			
			if ((x + y) % 2)
			{
				TileObject->SetMaterial(0); // Silver
			}
			else
			{
				TileObject->SetMaterial(1); // Black
			}

			TileObject->SetTileStatus(ETileStatus::EMPTY);
			TileObject->SetTileTeam(ETeam::NONE);
			TileObject->SetAlgebraicPosition(y, x);
			TileObject->SetTileLocation(SpawnLocation);
			GameMode->TileArray.Add(TileObject);
		}
	}
}

/*
 * Method used in the SpawnPieces to spawn a single ChessPiece
 * in the right position and set the Tile to OCCUPIED
 */
void AChessboard::SpawnPieces()
{
	// Get the reference to the Chess_GameMode
	AChess_GameMode* GameMode = Cast<AChess_GameMode>(GetWorld()->GetAuthGameMode());
	
	for (ATile* CurrentTile: GameMode->TileArray)
	{
		const TCHAR TileLetter = CurrentTile->GetAlgebraicPosition().TileLetter;
		const int32 TileNumber = CurrentTile->GetAlgebraicPosition().TileNumber;
		
		if (TileNumber == 1)
		{
			switch (TileLetter)
			{
			case 'a':
				{
					GameMode->WhiteTeam.Add(SpawnSinglePiece(CurrentTile, WHITE, ROOK));
					break;
				}
			case 'b':
				{
					GameMode->WhiteTeam.Add(SpawnSinglePiece(CurrentTile, WHITE, KNIGHT));
					break;
				}
			case 'c':
				{
					GameMode->WhiteTeam.Add(SpawnSinglePiece(CurrentTile, WHITE, BISHOP));
					break;
				}
			case 'd':
				{
					GameMode->WhiteTeam.Add(SpawnSinglePiece(CurrentTile, WHITE, QUEEN));
					break;
				}
			case 'e':
				{
					AChess_Piece* WhiteKing = SpawnSinglePiece(CurrentTile, WHITE, KING);
					GameMode->WhiteTeam.Add(WhiteKing);
					GameMode->Kings[WHITE] = Cast<AChess_King>(WhiteKing);
					break;
				}
			case 'f':
				{
					GameMode->WhiteTeam.Add(SpawnSinglePiece(CurrentTile, WHITE, BISHOP));
					break;
				}
			case 'g':
				{
					GameMode->WhiteTeam.Add(SpawnSinglePiece(CurrentTile, WHITE, KNIGHT));
					break;
				}
			case 'h':
				{
					GameMode->WhiteTeam.Add(SpawnSinglePiece(CurrentTile, WHITE, ROOK));
					break;
				}
			default: break;
			}
		}
		
		if (TileNumber == 2)
		{
			GameMode->WhiteTeam.Add(SpawnSinglePiece(CurrentTile, WHITE, PAWN));
		}

		if (TileNumber == 7)
		{
			GameMode->BlackTeam.Add(SpawnSinglePiece(CurrentTile, BLACK, PAWN));
		}

		if (TileNumber == 8)
		{
			switch (TileLetter)
			{
			case 'a':
				{
					GameMode->BlackTeam.Add(SpawnSinglePiece(CurrentTile, BLACK, ROOK));
					break;
				}
			case 'b':
				{
					GameMode->BlackTeam.Add(SpawnSinglePiece(CurrentTile, BLACK, KNIGHT));
					break;
				}
			case 'c':
				{
					GameMode->BlackTeam.Add(SpawnSinglePiece(CurrentTile, BLACK, BISHOP));
					break;
				}
			case 'd':
				{
					GameMode->BlackTeam.Add(SpawnSinglePiece(CurrentTile, BLACK, QUEEN));
					break;
				}
			case 'e':
				{
					AChess_Piece* BlackKing = SpawnSinglePiece(CurrentTile, BLACK, KING);
					GameMode->BlackTeam.Add(BlackKing);
					GameMode->Kings[BLACK] = Cast<AChess_King>(BlackKing);
					break;
				}
			case 'f':
				{
					GameMode->BlackTeam.Add(SpawnSinglePiece(CurrentTile, BLACK, BISHOP));
					break;
				}
			case 'g':
				{
					GameMode->BlackTeam.Add(SpawnSinglePiece(CurrentTile, BLACK, KNIGHT));
					break;
				}
			case 'h':
				{
					GameMode->BlackTeam.Add(SpawnSinglePiece(CurrentTile, BLACK, ROOK));
					break;
				}
			default: break;
			}
		}
	}
}

void AChessboard::SelfDestroy()
{
	Destroy();
}

// Called every frame
// void AChessboard::Tick(float DeltaTime)
// {
	// Super::Tick(DeltaTime);
// }

