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
};
