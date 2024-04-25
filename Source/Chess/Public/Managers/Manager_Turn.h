// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"
#include "Pieces/Chess_Piece.h"
#include "Manager_Turn.generated.h"

class AChess_Piece;
class ATile;

/**
 * Structure assigned to each MoveHistory button with the variables
 * relating to the move made in that turn
 */
USTRUCT(Blueprintable)
struct FMoveInfo
{
	GENERATED_BODY()

	UUserWidget* Button;
	AChess_Piece* MovedPiece;
	AChess_Piece* KilledPiece;
	AChess_Piece* PromotedPiece;
	ATile* PreviousTile;
	ATile* NextTile;
	ATile* RookPreviousTile;
	AChess_Piece* RookCastle;
	bool bJustMoved;
};

/**
 *	Class used as manager of each turn
 */
UCLASS()
class CHESS_API AManager_Turn : public AActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AManager_Turn();
	void SelfDestroy();
	
	void ResetVariables();
	void ResetTargetedAndKillableTiles();
	void ResetSelectedPiece() const;
	void SetTilesAndPieces(ATile* PTile, ATile* NTile, AChess_Piece* PieceToMove, AChess_Piece* PieceToKill, bool bIsJustMoved);
	void SetCastleReferences(ATile* PRookTile, AChess_Piece* RCastle);

	/*
	 * Game references for a turn
	 */
	AChess_Piece* MovedPiece;
	AChess_Piece* KilledPiece;
	AChess_Piece* PromotedPiece;
	ATile* PreviousTile;
	ATile* NextTile;

	ATile* RookPreviousTile;
	AChess_Piece* RookCastle;
	bool bJustMoved;

	/*
	 * Game variables for a turn
	 */
	bool bIsKill; // If in that turn there was a kill
	bool bIsPromotion; // If in that turn there was a promotion
	bool bIsBlackKingInCheck;
	bool bIsWhiteKingInCheck;

	bool bIsCastleLong;
	bool bIsCastleShort;
	
	TArray<ATile*> WhiteMoves; // Array with White's legal moves in the current turn
	TArray<ATile*> BlackMoves; // Array with Black's legal moves in the current turn

	UPROPERTY(Transient)
	TArray<ATile*> TargetedTiles; // Keeps track of Targeted Tiles
	UPROPERTY(Transient)
	TArray<ATile*> KillableTiles; // Keeps track of Killable Tiles
	UPROPERTY(Transient)
	AChess_Piece* SelectedPiece; // Keeps track of Selected Piece

	/*
	UPROPERTY(Transient)
	TArray<ATile*> CastlingTiles;
	*/
	TArray<AChess_Piece*> RooksToCastle;

	TPair<FString, int32> SaveGameState() const;

	/*
	 * Manage MoveHistory
	 */
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> MhButtonClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> EndButtonClass;

	TArray<FMoveInfo> MoveHistory;
	int32 CurrentButtonIndex;

	void DisplayMove();
	void DisplayEndGame() const;
	UFUNCTION(BlueprintCallable)
	FString ComputeNotation() const;

	void DisableReplay() const;
	void EnableReplay();

	void DestroyMoveHistory();

	UFUNCTION(BlueprintCallable)
	void Replay(const int32 ClickedIndex);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
