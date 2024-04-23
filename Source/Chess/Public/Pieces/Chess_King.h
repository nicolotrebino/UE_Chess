// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Chess_Piece.h"
#include "Chess_King.generated.h"

/**
 *	Chess_King class, derived from Chess_Piece class
 */
UCLASS()
class CHESS_API AChess_King : public AChess_Piece
{
	GENERATED_BODY()

public:
	AChess_King();
	
	/*
	*	Methods inherited from Chess_Piece and
	*	overridden for the King
	*/
	virtual TArray<ATile*> GetPossibleMoves() override;
	virtual void SetMaterial(const int32 Index) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/* 
	* Components
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* KingComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* KingMeshComponent;

	/*
	* Materials
	*/
	UPROPERTY(EditDefaultsOnly)
	TArray<UMaterialInstance*> KingMaterials;
};
