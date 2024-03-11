// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Chess_Piece.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "HUD_UserInterface.generated.h"

USTRUCT()
struct FHistoryButton
{
	GENERATED_BODY()

	UButton* Button;
	AChess_Piece* MovedPiece;
	ETeam KilledPieceTeam;
	EPieceType KilledPieceType;
	ATile* PreviousTile;
	ATile* NextTile;
};

// Macro declaration for a dynamic multicast delegate
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnReplay);

/**
 * 
 */
UCLASS()
class CHESS_API UHUD_UserInterface : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetNextTile(ATile* Tile);
	ATile* GetNextTile() const;
	void SetPreviousTile(ATile* Tile);
	ATile* GetPreviousTile() const;
	void SetPieceToMove(AChess_Piece* Piece);
	AChess_Piece* GetPieceToMove() const;
	void SetPieceToKill(AChess_Piece* KilledPiece);
	ETeam GetPieceToKillTeam() const;
	EPieceType GetPieceToKillType() const;

	FString ComputeNomenclature() const;
	void SaveMove(const FString& Nomenclature);
	void SaveEndGame(const FString& EndGame) const;

	void MakeHistoryArray(UButton* Button);

	UFUNCTION(BlueprintCallable)
	void Backtrack();

	UFUNCTION()
	void DestroyMoveHistory() const;

protected:
	ATile* NextTile = nullptr;
	ATile* PreviousTile = nullptr;
	AChess_Piece* PieceToMove = nullptr;
	ETeam KilledPieceTeam;
	EPieceType KilledPieceType;

	TArray<FHistoryButton> MhButtons;

	// BlueprintAssignable Usable with Multicast Delegates only. Exposes the property for assigning in Blueprints.
	// declare a variable of type FOnReset (delegate)
	UPROPERTY(BlueprintAssignable)
	FOnReplay OnReplayEvent;

	// TSubclassOf template class that provides UClass type safety
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UWidget> ButtonClass;
};
