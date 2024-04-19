// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Utility.h"
#include "GameFramework/Actor.h"
#include "Tile.generated.h"

class AChess_Piece;

USTRUCT()
struct FAlgebraicPosition
{
	GENERATED_BODY()

	TCHAR TileLetter;
	uint8 TileNumber;
};

/**
 * Tile to compose the Chessboard
 */
UCLASS()
class CHESS_API ATile : public AActor
{
	GENERATED_BODY()
	
public:	

	ATile();

	/*
	 * Setter and getter for Tile information
	 */
	void SetTileLocation(const FVector& Location);
	FVector GetTileLocation() const;
	void SetAlgebraicPosition(const int X, const int Y);
	FAlgebraicPosition GetAlgebraicPosition() const;
	void SetTileStatus(const ETileStatus TileStatus);
	ETileStatus GetTileStatus() const;
	void SetTileTeam(const ETeam TileTeam);
	ETeam GetTileTeam() const;

	/*
	 * Manage Piece on Tile
	 */
	void SetPieceOnTile(AChess_Piece* ChessPiece);
	AChess_Piece* GetPieceOnTile() const;

	/*
	 * Manage Tile material and its Status in game
	 */
	void SetMaterial(const uint8 Index);
	void SetTargetTile();
	void UnsetTargetTile();
	void SetSelectedTile() const;
	void UnsetSelectedTile();
	void SetKillableTile();
	void UnsetKillableTile();

protected:
	virtual void BeginPlay() override;

	/*
	 * Tile components
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* Scene;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* StaticMeshComponent;

	/*
	 * Tile materials
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Materials")
	TArray<UMaterialInstance*> DefaultMaterials;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Materials")
	TArray<UMaterialInstance*> TargetMaterials;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Materials")
	TArray<UMaterialInstance*> KillMaterials;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Materials")
	UMaterialInstance* SelectMaterial;
	uint8 TileMaterial; // Tile default material

	/*
	 * Tile information
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FVector TileLocation; // Tile location in the map
	ETileStatus Status; // Tile Status
	ETeam Team; // Team of the Piece on Tile
	FAlgebraicPosition AlgebraicPosition; // Position with Algebraic Notation
	AChess_Piece* CurrentPieceOnTile; // Piece on Tile
	
	UFUNCTION()
	void SelfDestroy();
};
