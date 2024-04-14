// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Chess_Piece.h"
#include "Chess_Bishop.generated.h"

/**
 * 
 */
UCLASS()
class CHESS_API AChess_Bishop : public AChess_Piece
{
	GENERATED_BODY()

public:
	// Default constructor
	AChess_Bishop();

	virtual TArray<ATile*> GetPossibleMoves() override;
	virtual void SetMaterial(const int32 Index) override;

protected:
	virtual void BeginPlay() override;

	/* 
	* Components
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* BishopComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* BishopMeshComponent;

	/*
	* Materials
	*/
	UPROPERTY(EditDefaultsOnly)
	TArray<UMaterialInstance*> BishopMaterials;
};
