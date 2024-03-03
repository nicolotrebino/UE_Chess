// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Chess/Chess.h"
#include "GameFramework/Actor.h"
#include "Tile.generated.h"

USTRUCT()
struct FChessPosition
{
	GENERATED_BODY()

	TCHAR TileLetter;
	uint8 TileNumber;
	
};

UCLASS()
class CHESS_API ATile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATile();

	/*
	 * Setter and getter for the Tile location
	 */
	void SetTileLocation(const FVector& Location);
	FVector GetTileLocation() const;

	/* 
	 * Setter and getter for Tile information
	 */
	void SetChessPosition(const int X, const int Y);
	FChessPosition GetChessPosition() const;
	
	void SetTileStatus(const ETileStatus TileStatus);
	ETileStatus GetTileStatus() const;
	
	void SetTileTeam(const EPieceTeam TileTeam);
	EPieceTeam GetTileTeam() const;

	// Manage Piece on Tile
	void SetPieceOnTile(AChessPiece* ChessPiece);
	AChessPiece* GetPieceOnTile() const;

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
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/* 
	 * Components
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* Scene;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* StaticMeshComponent;

	/*
	 * Materials
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
	UPROPERTY(VisibleAnywhere, Category = "Information")
	ETileStatus Status; // Tile Status
	UPROPERTY(VisibleAnywhere, Category = "Information")
	EPieceTeam Team; // Team of the Piece on Tile
	UPROPERTY(VisibleAnywhere, Category = "Information")
	FChessPosition ChessPosition; // Position with Algebraic Notation

	UPROPERTY(VisibleAnywhere, Category = "Information")
	AChessPiece* CurrentPieceOnTile; // Piece on Tile
	
	UFUNCTION()
	void SelfDestroy();

/*
* It shouldn't tick
*/
// public:
	// Called every frame
	// virtual void Tick(float DeltaTime) override;

};
