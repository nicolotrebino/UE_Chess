// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Chess_Piece.h"
#include "Tile.h"
#include "GameFramework/Actor.h"
#include "Chessboard.generated.h"

UCLASS()
class CHESS_API AChessboard : public AActor
{
	GENERATED_BODY()
	
public:
	
	AChessboard(); // Sets default values for this actor's properties

	// Return a (x,y) position given a hit (click) on a field tile
	FVector GetTilePosition(const FHitResult& Hit) const;
	
	// Getter for Chessboard size information
	int32 GetFieldSize() const;
	float GetTileSize() const;

	// Public function to spawn a single Piece, used also for the Pawn promotion
	UFUNCTION()
	AChess_Piece* SpawnSinglePiece(ATile* CurrentTile, const ETeam Team, const EPieceType Type);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/* Tile and Pieces classes */
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ATile> TileClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<TSubclassOf<AChess_Piece>> PieceClass;

	/* Chessboard size information */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 FieldSize;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float TileSize;
	
	UFUNCTION()
	void GenerateChessBoard() const; // Spawn Chessboard
	UFUNCTION()
	void SpawnPieces(); // Spawn all the Pieces on the Chessboard
	
	UFUNCTION()
	void SelfDestroy();

/* It shouldn't tick */
// public:	
	// Called every frame
	// virtual void Tick(float DeltaTime) override;

};
