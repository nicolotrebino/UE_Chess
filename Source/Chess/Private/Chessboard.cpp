// Fill out your copyright notice in the Description page of Project Settings.

#include "Chessboard.h"

#include "Chess_GameMode.h"
//////////////////////////////////// #include "Chess_King.h"

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
	// AChessGameMode* GameMode = Cast<AChessGameMode>(GetWorld()->GetAuthGameMode());

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

	// Get the Chess_GameMode instance with Singleton pattern
	AChess_GameMode* GameMode = AChess_GameMode::GetChessGameMode();
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
	// Get the Chess_GameMode instance with Singleton pattern
	AChess_GameMode* GameMode = AChess_GameMode::GetChessGameMode();
	
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
				TileObject->SetMaterial(0);
			}
			else
			{
				TileObject->SetMaterial(1);
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
 *  in the right position and set the Tile to OCCUPIED
 */
void AChessboard::SpawnPieces()
{
	// Get the Chess_GameMode instance with Singleton pattern
	AChess_GameMode* GameMode = AChess_GameMode::GetChessGameMode();
	
	for (ATile* CurrentTile: GameMode->TileArray)
	{
		if (CurrentTile->GetAlgebraicPosition().TileNumber == 1)
		{
			switch (CurrentTile->GetAlgebraicPosition().TileLetter)
			{
			case 'a':
				SpawnSinglePiece(CurrentTile, ETeam::WHITE, EPieceType::ROOK);
				break;
			case 'b':
				SpawnSinglePiece(CurrentTile, ETeam::WHITE, EPieceType::KNIGHT);
				break;
			case 'c':
				SpawnSinglePiece(CurrentTile, ETeam::WHITE, EPieceType::BISHOP);
				break;
			case 'd':
				SpawnSinglePiece(CurrentTile, ETeam::WHITE, EPieceType::QUEEN);
				break;
			case 'e':
				SpawnSinglePiece(CurrentTile, ETeam::WHITE, EPieceType::KING);
				break;
			case 'f':
				SpawnSinglePiece(CurrentTile, ETeam::WHITE, EPieceType::BISHOP);
				break;
			case 'g':
				SpawnSinglePiece(CurrentTile, ETeam::WHITE, EPieceType::KNIGHT);
				break;
			case 'h':
				SpawnSinglePiece(CurrentTile, ETeam::WHITE, EPieceType::ROOK);
				break;
			default:
				break;
			}
		}
		
		if (CurrentTile->GetAlgebraicPosition().TileNumber == 2)
		{
			SpawnSinglePiece(CurrentTile, ETeam::WHITE, EPieceType::PAWN);
		}

		if (CurrentTile->GetAlgebraicPosition().TileNumber == 7)
		{
			SpawnSinglePiece(CurrentTile, ETeam::WHITE, EPieceType::PAWN);
		}

		if (CurrentTile->GetAlgebraicPosition().TileNumber == 8)
		{
			switch (CurrentTile->GetAlgebraicPosition().TileLetter)
			{
			case 'a':
				SpawnSinglePiece(CurrentTile, ETeam::BLACK, EPieceType::ROOK);
				break;
			case 'b':
				SpawnSinglePiece(CurrentTile, ETeam::BLACK, EPieceType::KNIGHT);
				break;
			case 'c':
				SpawnSinglePiece(CurrentTile, ETeam::BLACK, EPieceType::BISHOP);
				break;
			case 'd':
				SpawnSinglePiece(CurrentTile, ETeam::BLACK, EPieceType::QUEEN);
				break;
			case 'e':
				SpawnSinglePiece(CurrentTile, ETeam::BLACK, EPieceType::KING);
				break;
			case 'f':
				SpawnSinglePiece(CurrentTile, ETeam::BLACK, EPieceType::BISHOP);
				break;
			case 'g':
				SpawnSinglePiece(CurrentTile, ETeam::BLACK, EPieceType::KNIGHT);
				break;
			case 'h':
				SpawnSinglePiece(CurrentTile, ETeam::BLACK, EPieceType::ROOK);
				break;
			default:
				break;
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

