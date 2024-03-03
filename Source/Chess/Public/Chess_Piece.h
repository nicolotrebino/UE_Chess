// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tile.h"
#include "Chess/Chess.h"
#include "GameFramework/Actor.h"
#include "Chess_Piece.generated.h"

UCLASS()
class CHESS_API AChess_Piece : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AChess_Piece();

	/*
	 * Setter and getter for the Piece information
	 */
	TCHAR GetNomenclature() const;
	void SetTeam(const EPieceTeam Team);
	EPieceTeam GetTeam();
	void SetType(const EPieceType Type);
	EPieceType GetType() const;
	void SetPieceTile(ATile* Tile);
	ATile* GetPieceTile() const;
	void SetPieceLocation(const FVector& Location);
	FVector GetPieceLocation() const;

	/*
	 * Functions that return legal moves
	 */
	virtual TArray<ATile*> GetPossibleMoves() { return TArray<ATile*>(); };
	virtual void PossibleMovesCheckControl(TArray<ATile*>& PossibleMoves); 
	TArray<ATile*> GetLegalMoves();

	/*
	 * Functions to move the piece and "eat" opponents
	 */
	virtual void MovePiece(ATile* NextTile);
	void Kill(const EPieceTeam Team, AChessPiece* Enemy) const;

	// Set the right material for the Piece
	virtual void SetMaterial(const int32 Index) {};
	
	UFUNCTION()
	void SelfDestroy(); // Destroy a ChessPiece actor

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Reference to the Chess_GameMode
	AChessGameMode* GameMode;
	
	/*
	 * Piece information
	 */
	TCHAR Nomenclature; // Capital letter to indicate the Piece for the Algebraic notation
	EPieceTeam PieceTeam;
	EPieceType PieceType;
	ATile* CurrentTile; // Tile under the Piece
	FVector PieceLocation; // Piece location in the map

	/*
	 * Get all the possible moves in the vertical,
	 * horizontal or diagonal lines
	 */
	TArray<ATile*> GetVerticalLine() const;
	TArray<ATile*> GetHorizontalLine() const;
	TArray<ATile*> GetDiagonalLine() const;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

inline void AChess_Piece::SelfDestroy()
{
	Destroy();
}
