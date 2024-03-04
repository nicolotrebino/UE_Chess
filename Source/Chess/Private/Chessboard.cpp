// Fill out your copyright notice in the Description page of Project Settings.


#include "Chessboard.h"

#include "ChessGameMode.h"
#include "ChessKing.h"

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

void AChessboard::SpawnSinglePiece(ATile* CurrentTile, const EPieceTeam Team, const EPieceType Type)
{
	AChess_GameMode* GameMode = Cast<AChess_GameMode>(GetWorld()->GetAuthGameMode());

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
	
	switch (Team)
	{
	case EPieceTeam::WHITE:
		GameMode->WhiteTeam.Add(Piece);
		if (Type == EPieceType::KING)
		{
			GameMode->Kings[EPieceTeam::WHITE] = Cast<AChess_King>(Piece);
		}
		break;
	case EPieceTeam::BLACK:
		GameMode->BlackTeam.Add(Piece);
		if (Type == EPieceType::KING)
		{
			GameMode->Kings[EPieceTeam::BLACK] = Cast<AChess_King>(Piece);
		}
		break;
	default:
		break;
	}
}

// Called when the game starts or when spawned
void AChessboard::BeginPlay()
{
	Super::BeginPlay();

	// Add SelfDestroy method to the broadcast OnResetEvent delegate
	AChess_GameMode* GameMode = Cast<AChess_GameMode>(GetWorld()->GetAuthGameMode());
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
				TileObject->SetMaterial(0);
			}
			else
			{
				TileObject->SetMaterial(1);
			}

			TileObject->SetTileStatus(ETileStatus::EMPTY);
			TileObject->SetTileTeam(EPieceTeam::NONE);
			TileObject->SetChessPosition(y, x);
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
	AChess_GameMode* GameMode = Cast<AChess_GameMode>(GetWorld()->GetAuthGameMode());
	
	for (ATile* CurrentTile: GameMode->TileArray)
	{
		if (CurrentTile->GetChessPosition().TileNumber == 1)
		{
			switch (CurrentTile->GetChessPosition().TileLetter)
			{
			case 'a':
				SpawnSinglePiece(CurrentTile, EPieceTeam::WHITE, EPieceType::ROOK);
				break;
			case 'b':
				SpawnSinglePiece(CurrentTile, EPieceTeam::WHITE, EPieceType::KNIGHT);
				break;
			case 'c':
				SpawnSinglePiece(CurrentTile, EPieceTeam::WHITE, EPieceType::BISHOP);
				break;
			case 'd':
				SpawnSinglePiece(CurrentTile, EPieceTeam::WHITE, EPieceType::QUEEN);
				break;
			case 'e':
				SpawnSinglePiece(CurrentTile, EPieceTeam::WHITE, EPieceType::KING);
				break;
			case 'f':
				SpawnSinglePiece(CurrentTile, EPieceTeam::WHITE, EPieceType::BISHOP);
				break;
			case 'g':
				SpawnSinglePiece(CurrentTile, EPieceTeam::WHITE, EPieceType::KNIGHT);
				break;
			case 'h':
				SpawnSinglePiece(CurrentTile, EPieceTeam::WHITE, EPieceType::ROOK);
				break;
			default:
				break;
			}
		}
		
		if (CurrentTile->GetChessPosition().TileNumber == 2)
		{
			SpawnSinglePiece(CurrentTile, EPieceTeam::WHITE, EPieceType::PAWN);
		}

		if (CurrentTile->GetChessPosition().TileNumber == 7)
		{
			SpawnSinglePiece(CurrentTile, EPieceTeam::WHITE, EPieceType::PAWN);
		}

		if (CurrentTile->GetChessPosition().TileNumber == 8)
		{
			switch (CurrentTile->GetChessPosition().TileLetter)
			{
			case 'a':
				SpawnSinglePiece(CurrentTile, EPieceTeam::BLACK, EPieceType::ROOK);
				break;
			case 'b':
				SpawnSinglePiece(CurrentTile, EPieceTeam::BLACK, EPieceType::KNIGHT);
				break;
			case 'c':
				SpawnSinglePiece(CurrentTile, EPieceTeam::BLACK, EPieceType::BISHOP);
				break;
			case 'd':
				SpawnSinglePiece(CurrentTile, EPieceTeam::BLACK, EPieceType::QUEEN);
				break;
			case 'e':
				SpawnSinglePiece(CurrentTile, EPieceTeam::BLACK, EPieceType::KING);
				break;
			case 'f':
				SpawnSinglePiece(CurrentTile, EPieceTeam::BLACK, EPieceType::BISHOP);
				break;
			case 'g':
				SpawnSinglePiece(CurrentTile, EPieceTeam::BLACK, EPieceType::KNIGHT);
				break;
			case 'h':
				SpawnSinglePiece(CurrentTile, EPieceTeam::BLACK, EPieceType::ROOK);
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

