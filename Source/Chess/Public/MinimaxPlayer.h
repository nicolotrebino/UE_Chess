// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Chess_GameInstance.h"
#include "Chess_Piece.h"
#include "PlayerInterface.h"
#include "GameFramework/Pawn.h"
#include "MinimaxPlayer.generated.h"

USTRUCT()
struct FNextMove
{
	GENERATED_BODY()

	AChess_Piece* PieceToMove;
	ATile* NextTile;
};

UCLASS()
class CHESS_API AMinimaxPlayer : public APawn, public IPlayerInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AMinimaxPlayer();

	UChess_GameInstance* GameInstance;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void OnTurn() override;
	virtual void OnWin() override;
	virtual void OnLose() override;

	int32 EvaluateGrid() const;
	int32 AlphaBetaMiniMax(int32 Depth, int32 Alpha, int32 Beta, bool IsMax);
	FNextMove FindBestMove();

private:
	const int32 BPawns[8][8] = {
		{ 0,  0,  0,  0,  0,  0,  0,  0 },
		{ 5, 10, 10, -20, -20, 10, 10,  5 },
		{ 5, -5, -10,  0,  0, -10, -5,  5 },
		{ 0,  0,  0, 20, 20,  0,  0,  0 },
		{ 5,  5, 10, 25, 25, 10,  5,  5 },
		{ 10, 10, 20, 30, 30, 20, 10, 10 },
		{ 50, 50, 50, 50, 50, 50, 50, 50 },
		{ 0,  0,  0,  0,  0,  0,  0,  0 }
	};
	
	const int32 BKnights[8][8] = {

		{-50, -40, -30, -30, -30, -30, -40, -50},
		{-40, -20,   0,   5,   5,   0, -20, -40},
		{-30,   5,  10,  15,  15,  10,   5, -30},
		{-30,   0,  15,  20,  20,  15,   0, -30},
		{-30,   5,  15,  20,  20,  15,   5, -30},
		{-30,   0,  10,  15,  15,  10,   0, -30},
		{-40, -20,   0,   0,   0,   0, -20, -40},
		{-50, -40, -30, -30, -30, -30, -40, -50}
	};
	
	const int32 BBishops[8][8] = {
		{-20, -10, -10, -10, -10, -10, -10, -20},
		{-10,   5,   0,   0,   0,   0,   5, -10},
		{-10,  10,  10,  10,  10,  10,  10, -10},
		{-10,   0,  10,  10,  10,  10,   0, -10},
		{-10,   5,   5,  10,  10,   5,   5, -10},
		{-10,   0,   5,  10,  10,   5,   0, -10},
		{-10,   0,   0,   0,   0,   0,   0, -10},
		{-20, -10, -10, -10, -10, -10, -10, -20}
	};
	
	const int32 BRooks[8][8] = {
		{  0,   0,   0,   5,   5,   0,   0,   0},
		{ -5,   0,   0,   0,   0,   0,   0,  -5},
		{ -5,   0,   0,   0,   0,   0,   0,  -5},
		{ -5,   0,   0,   0,   0,   0,   0,  -5},
		{ -5,   0,   0,   0,   0,   0,   0,  -5},
		{ -5,   0,   0,   0,   0,   0,   0,  -5},
		{  5,  10,  10,  10,  10,  10,  10,   5},
		{  0,   0,   0,   0,   0,   0,   0,   0}
	};
	
	const int32 BQueens[8][8] = {
		{-20, -10, -10,  -5,  -5, -10, -10, -20},
		{-10,   0,   5,   0,   0,   0,   0, -10},
		{-10,   5,   5,   5,   5,   5,   0, -10},
		{  0,   0,   5,   5,   5,   5,   0,  -5},
		{ -5,   0,   5,   5,   5,   5,   0,  -5},
		{-10,   0,   5,   5,   5,   5,   0, -10},
		{-10,   0,   0,   0,   0,   0,   0, -10},
		{-20, -10, -10,  -5,  -5, -10, -10, -20}
	};

	// Pawns
	const int32 WPawns[8][8] = {
		{ 0,  0,  0,  0,  0,  0,  0,  0},
		{50, 50, 50, 50, 50, 50, 50, 50},
		{10, 10, 20, 30, 30, 20, 10, 10},
		{ 5,  5, 10, 25, 25, 10,  5,  5},
		{ 0,  0,  0, 20, 20,  0,  0,  0},
		{ 5, -5,-10,  0,  0,-10, -5,  5},
		{ 5, 10, 10,-20,-20, 10, 10,  5},
		{ 0,  0,  0,  0,  0,  0,  0,  0}
	};

	// Knight
	const int32 WKnights[8][8] = {
		{-50,-40,-30,-30,-30,-30,-40,-50},
		{-40,-20,  0,  0,  0,  0,-20,-40},
		{-30,  0, 10, 15, 15, 10,  0,-30},
		{-30,  5, 15, 20, 20, 15,  5,-30},
		{-30,  0, 15, 20, 20, 15,  0,-30},
		{-30,  5, 10, 15, 15, 10,  5,-30},
		{-40,-20,  0,  5,  5,  0,-20,-40},
		{-50,-40,-30,-30,-30,-30,-40,-50}
	};

	// Bishop
	const int32 WBishops[8][8] = {
		{-20,-10,-10,-10,-10,-10,-10,-20},
		{-10,  0,  0,  0,  0,  0,  0,-10},
		{-10,  0,  5, 10, 10,  5,  0,-10},
		{-10,  5,  5, 10, 10,  5,  5,-10},
		{-10,  0, 10, 10, 10, 10,  0,-10},
		{-10, 10, 10, 10, 10, 10, 10,-10},
		{-10,  5,  0,  0,  0,  0,  5,-10},
		{-20,-10,-10,-10,-10,-10,-10,-20}
	};

	// Rook
	const int32 WRooks[8][8] = {
		{ 0,  0,  0,  0,  0,  0,  0,  0},
		{ 5, 10, 10, 10, 10, 10, 10,  5},
		{-5,  0,  0,  0,  0,  0,  0, -5},
		{-5,  0,  0,  0,  0,  0,  0, -5},
		{-5,  0,  0,  0,  0,  0,  0, -5},
		{-5,  0,  0,  0,  0,  0,  0, -5},
		{-5,  0,  0,  0,  0,  0,  0, -5}
	};

	// Queen
	const int32 WQueens[8][8] = {
		{-20,-10,-10, -5, -5,-10,-10,-20},
		{-10,  0,  0,  0,  0,  0,  0,-10},
		{-10,  0,  5,  5,  5,  5,  0,-10},
		{ -5,  0,  5,  5,  5,  5,  0, -5},
		{  0,  0,  5,  5,  5,  5,  0, -5},
		{-10,  5,  5,  5,  5,  5,  0,-10},
		{-10,  0,  5,  0,  0,  0,  0,-10},
		{-20,-10,-10, -5, -5,-10,-10,-20}
	};
};
