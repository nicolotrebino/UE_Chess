// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tile.h"
#include "Utility.h"
#include "GameFramework/Actor.h"
#include "Chess_Piece.generated.h"

class AChess_GameMode;

/**
 * Father class for chess pieces
 */
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
	virtual void SetTeam(const ETeam Team);
	virtual ETeam GetTeam();
	virtual void SetType(const EPieceType Type);
	virtual EPieceType GetType();
	virtual void SetPieceTile(ATile* Tile);
	virtual ATile* GetPieceTile();
	virtual void SetPieceLocation(const FVector& Location);
	virtual FVector GetPieceLocation();
	virtual int32 GetPieceValue() const;

	/*
	 * Functions that return legal moves
	 */
	virtual TArray<ATile*> GetPossibleMoves() { return TArray<ATile*>(); }; // Extended in the children
	void PossibleMovesCheckControl(TArray<ATile*>& PossibleMoves);
	void CheckKingMobility(TArray<ATile*> &PossibleMoves);

	bool CanKingCastleShort() const;
	bool CanKingCastleLong() const;
	
	TArray<ATile*> GetLegitMoves();

	/*
	 * Functions to move the piece and "eat" opponents
	 */
	virtual void MovePiece(ATile* NextTile); // Extended in Chess_Pawn
	void Kill(AChess_Piece* Enemy) const;

	void VirtualMove(ATile* NextTile, ATile* PreviousTile, AChess_Piece* Killed);
	void VirtualUnMove(ATile* NextTile, ATile* PreviousTile, AChess_Piece* Killed);

	// Set the right material for the Piece
	virtual void SetMaterial(const int32 Index) {};
	
	UFUNCTION()
	void SelfDestroy(); // Destroy a ChessPiece actor

	TArray<ATile*> MyLegalMoves;

	bool bAlreadyMoved;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	AChess_GameMode* GameMode; // Reference to the Chess_GameMode
	
	/*
	 * Piece information
	 */
	TCHAR Nomenclature; // Capital letter to indicate the Piece for the Algebraic notation
	ETeam PieceTeam;
	EPieceType PieceType;
	ATile* CurrentTile; // Tile under the Piece
	FVector PieceLocation; // Piece location in the map
	int32 PieceValue;

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
