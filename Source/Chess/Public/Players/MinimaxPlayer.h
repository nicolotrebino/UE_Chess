// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Chess_GameInstance.h"
#include "Pieces/Chess_Piece.h"
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

	/*
	 *	Manage virtual promotion for the minimax algorithm
	 */
	bool bIsVirtualPromotion = false;
	AChess_Piece* PiecePromoted = nullptr;
	AChess_Piece* NewQueen = nullptr;

	/*
	 *	Methods inherited from PlayerInterface and
	 *	overridden for the MinimaxPlayer
	 */	
	virtual void OnTurn() override;
	virtual void OnWin() override;
	virtual void OnLose() override;
	virtual void OnDraw() override;

	/*
	 *	Minimax algorithm methods
	 */
	int32 EvaluateGrid() const;
	int32 AlphaBetaMiniMax(int32 D, int32 A, int32 B, bool IsMax);
	FNextMove FindBestMove();

private:
	/*
	 *	Minimax + Alpha Beta Pruning values
	 */
	int32 BestVal = -50000;
	int32 Alpha = -50000;
	int32 Beta = 50000;
	int32 Depth = 2;
	
	/*
	 *	Arrays to evaluate pieces position
	 */
	int32 Pawns[64] = {
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 15, 15, 0, 0, 0,
		0, 0, 0, 10, 10, 0, 0, 0,
		0, 0, 0, 5, 5, 0, 0, 0,
		0, 0, 0, -25, -25, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0
	  };

	int32 Knight[64] = {
		-40, -25, -25, -25, -25, -25, -25, -40,
		-30, 0, 0, 0, 0, 0, 0, -30,
		-30, 0, 0, 0, 0, 0, 0, -30,
		-30, 0, 0, 15, 15, 0, 0, -30,
		-30, 0, 0, 15, 15, 0, 0, -30,
		-30, 0, 10, 0, 0, 10, 0, -30,
		-30, 0, 0, 5, 5, 0, 0, -30,
		-40, -30, -25, -25, -25, -25, -30, -40
	  };

	int32 Bishops[64] = {
		-10, 0, 0, 0, 0, 0, 0, -10,
		-10, 5, 0, 0, 0, 0, 5, -10,
		-10, 0, 5, 0, 0, 5, 0, -10,
		-10, 0, 0, 10, 10, 0, 0, -10,
		-10, 0, 5, 10, 10, 5, 0, -10,
		-10, 0, 5, 0, 0, 5, 0, -10,
		-10, 5, 0, 0, 0, 0, 5, -10,
		-10, -20, -20, -20, -20, -20, -20, -10
	  };

	int32 Rooks[64] = {
		0, 0, 0, 0, 0, 0, 0, 0,
		10, 10, 10, 10, 10, 10, 10, 10,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 5, 5, 0, 0, 0
	  };

	int32 Kings[64] = {
		-25, -25, -25, -25, -25, -25, -25, -25,
		-25, -25, -25, -25, -25, -25, -25, -25,
		-25, -25, -25, -25, -25, -25, -25, -25,
		-25, -25, -25, -25, -25, -25, -25, -25,
		-25, -25, -25, -25, -25, -25, -25, -25,
		-25, -25, -25, -25, -25, -25, -25, -25,
		-25, -25, -25, -25, -25, -25, -25, -25,
		10, 15, -15, -15, -15, -15, 15, 10
	  };

	int32 Flip[64] = {
		56, 57, 58, 59, 60, 61, 62, 63,
		48, 49, 50, 51, 52, 53, 54, 55,
		40, 41, 42, 43, 44, 45, 46, 47,
		32, 33, 34, 35, 36, 37, 38, 39,
		24, 25, 26, 27, 28, 29, 30, 31,
		16, 17, 18, 19, 20, 21, 22, 23,
		8, 9, 10, 11, 12, 13, 14, 15,
		0, 1, 2, 3, 4, 5, 6, 7
	  };
};
