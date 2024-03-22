// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Chess_Piece.h"
#include "GameFramework/Actor.h"
#include "Manager_Turn.generated.h"

class AChess_Piece;
class ATile;

USTRUCT(Blueprintable)
struct FMoveInfo
{
	GENERATED_BODY()

	UUserWidget* Button;
	AChess_Piece* MovedPiece;
	AChess_Piece* KilledPiece;
	ATile* PreviousTile;
	ATile* NextTile;
};

UCLASS()
class CHESS_API AManager_Turn : public AActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AManager_Turn();
	void SelfDestroy();

	/* Methods */
	void ResetVariables();
	void SetTilesAndPieces(ATile* PTile, ATile* NTile, AChess_Piece* PieceToMove, AChess_Piece* PieceToKill);

	/* Game references for a turn */
	AChess_Piece* Checker; // Piece that is in check

	AChess_Piece* MovedPiece;
	AChess_Piece* KilledPiece;

	ATile* PreviousTile;
	ATile* NextTile;

	/* Game variables for a turn */
	bool bIsKill; // If in that turn there was a kill
	bool bIsPromotion; // If in that turn there was a promotion
	bool bIsBlackKingInCheck;
	bool bIsWhiteKingInCheck;

	/* Move History managing */
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

	void DestroyMoveHistory() const;

	UFUNCTION(BlueprintCallable)
	void Replay(const int32 ClickedIndex);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
