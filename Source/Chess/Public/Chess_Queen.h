// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Chess_Piece.h"
#include "Chess_Queen.generated.h"

/**
 * 
 */
UCLASS()
class CHESS_API AChess_Queen : public AChess_Piece
{
	GENERATED_BODY()

public:
	AChess_Queen();

	virtual TArray<ATile*> GetPossibleMoves() override;
	virtual void SetMaterial(const int32 Index) override;

protected:
	virtual void BeginPlay() override;

	/* 
	* Components
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* QueenComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* QueenMeshComponent;

	/*
	* Materials
	*/	
	UPROPERTY(EditDefaultsOnly)
	TArray<UMaterialInstance*> QueenMaterials;
	
};
