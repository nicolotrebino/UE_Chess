// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Chess_Piece.h"
#include "Chess_Pawn.generated.h"

class UManager_PromotionHUD;

/**
 * 
 */
UCLASS()
class CHESS_API AChess_Pawn : public AChess_Piece
{
	GENERATED_BODY()

public:
	AChess_Pawn();

	virtual TArray<ATile*> GetPossibleMoves() override;
	virtual void SetMaterial(const int32 Index) override;
	virtual void MovePiece(ATile* NextTile) override;

protected:
	virtual void BeginPlay() override;

	/* Components */	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* PawnComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* PawnMeshComponent;

	/* Materials */	
	UPROPERTY(EditDefaultsOnly)
	TArray<UMaterialInstance*> PawnMaterials;
};