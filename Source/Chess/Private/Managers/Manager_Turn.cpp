// Fill out your copyright notice in the Description page of Project Settings.

#include "Managers/Manager_Turn.h"

#include "Chess_GameInstance.h"
#include "Chess_GameMode.h"
#include "Chess_PlayerController.h"
#include "Pieces/Chess_Rook.h"
#include "Blueprint/WidgetTree.h"
#include "Components/Button.h"
#include "Components/HorizontalBox.h"
#include "Components/VerticalBox.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AManager_Turn::AManager_Turn()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MovedPiece = nullptr;
	KilledPiece = nullptr;
	PromotedPiece = nullptr;

	RookPreviousTile = nullptr;
	RookCastle = nullptr;

	bJustMoved = false;

	bIsKill = false;
	bIsPromotion = false;
	bIsBlackKingInCheck = false;
	bIsWhiteKingInCheck = false;

	bIsCastleLong = false;
	bIsCastleShort = false;

	// RookToCastle = nullptr;

	PreviousTile = nullptr;
	NextTile = nullptr;

	SelectedPiece = nullptr;

	CurrentButtonIndex = 0;
}

/*
 *	@brief	Destruct this manager before returning to the main menu
 *
 *	@return	Void
 */
void AManager_Turn::SelfDestroy()
{
	Destroy();
}

/*
 *	@brief	Every turn it resets the turn variables
 *
 *	@return Void
 */
void AManager_Turn::ResetVariables()
{
	MovedPiece = nullptr;
	KilledPiece = nullptr;
	PromotedPiece = nullptr;

	RookPreviousTile = nullptr;
	RookCastle = nullptr;

	bJustMoved = false;
	
	bIsKill = false;
	bIsPromotion = false;
	bIsBlackKingInCheck = false;
	bIsWhiteKingInCheck = false;

	PreviousTile = nullptr;
	NextTile = nullptr;

	bIsCastleLong = false;
	bIsCastleShort = false;
	
	// RookToCastle = nullptr;
}

/*
 *	@brief	Every turn it sets the turn variables
 *
 *	@param	PTile: tile where the piece started from before the move
 *	@param	NTile: tile where the piece has gone after the move
 *	@param	PieceToMove: piece moved during the turn
 *	@param	PieceToKill: killed piece during the turn
 *
 *	@return Void
 */
void AManager_Turn::SetTilesAndPieces(ATile* PTile, ATile* NTile, AChess_Piece* PieceToMove, AChess_Piece* PieceToKill, bool bIsJustMoved)
{
	PreviousTile = PTile;
	NextTile = NTile;
	MovedPiece = PieceToMove;
	KilledPiece = PieceToKill;
	bJustMoved = bIsJustMoved;
}

void AManager_Turn::SetCastleReferences(ATile* PRookTile, AChess_Piece* RCastle)
{
	RookPreviousTile = PRookTile;
	RookCastle= RCastle;
}

/*
 *	@brief	Bring back to the default color all the targeted Tiles
 *			and empty the TArray with all the pointers to the TargetedTiles
 *			
 *	@return Void
 */
void AManager_Turn::ResetTargetedAndKillableTiles()
{
	// Reset the targeted tiles and remove them from the "global" array
	for (ATile* Tile : TargetedTiles)
	{
		Tile->UnsetTargetTile(); // Make it EMPTY and with the default material
	}
	TargetedTiles.Empty();

	// Reset the killable tiles and remove them from the "global" array
	for (ATile* Tile : KillableTiles)
	{
		Tile->UnsetKillableTile(); // Make it OCCUPIED and with the default material
	}
	KillableTiles.Empty();

	/*
	for (ATile* Tile: CastlingTiles)
	{
		Tile->UnsetCastleTile();
	}
	CastlingTiles.Empty();
	*/
}

/*
 *	@brief	Bring back to the default color the Tile
 *			of the SelectedPiece
 *			
 *	@return Void
 */
void AManager_Turn::ResetSelectedPiece() const
{
	if (SelectedPiece)
	{
		SelectedPiece->GetPieceTile()->UnsetSelectedTile();
	}
}

/*
 *	@brief	Creates a string, inserts it as a key into a key-value pair
 *			used to store the state of the board during the game
 *			
 *	@return Value key pair with default value of 0
 */
TPair<FString, int32> AManager_Turn::SaveGameState() const
{
	AChess_GameMode* GameMode = Cast<AChess_GameMode>(GetWorld()->GetAuthGameMode());
	TPair<FString, int32> Pair;
	FString State;
	for (ATile* Tile: GameMode->TileArray)
	{
		if (Tile->GetPieceOnTile())
		{
			State.AppendChar(Tile->GetPieceOnTile()->GetNomenclature());
			if (Tile->GetTileTeam() == ETeam::WHITE)
			{
				State.AppendChar('W');
			}
			else
			{
				State.AppendChar('B');
			}
		}
		else
		{
			State.AppendChar('E');
		}
	}

	Pair.Key = State;
	Pair.Value = 0;
	
	return Pair;
}

/*
 *	@brief	Creates the correct button to insert into the MoveHistory for the move
 *			
 *	@return Void
 */
void AManager_Turn::DisplayMove()
{
	AChess_GameMode* GameMode = Cast<AChess_GameMode>(GetWorld()->GetAuthGameMode());
	AChess_PlayerController* Cpc = Cast<AChess_PlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	if (GameMode->bInReplay)
	{
		int32 i = MoveHistory.Num() - 1;
		while (i >= 0 && i != CurrentButtonIndex)
		{
			MoveHistory[i].Button->RemoveFromParent();
			MoveHistory[i].Button = nullptr;
			MoveHistory.RemoveAt(i);
			GameMode->MoveCounter = GameMode->MoveCounter - 1;
			GameMode->GameStates.RemoveAt(GameMode->GameStates.Num() - 1);
			
			i--;
		}
	}
	
	UUserWidget* ButtonWidget = Cpc->UserInterfaceWidget->WidgetTree->ConstructWidget<UUserWidget>(MhButtonClass);
	FMoveInfo Move = {ButtonWidget, MovedPiece, KilledPiece, PromotedPiece, PreviousTile, NextTile, RookPreviousTile, RookCastle, bJustMoved};
	MoveHistory.Add(Move);
	CurrentButtonIndex = GameMode->MoveCounter - 1;
	
	// If it is the AI, the move was done by the Human Player
	if (GameMode->CurrentPlayer)
	{
		UVerticalBox* Container = Cast<UVerticalBox>(Cpc->UserInterfaceWidget->WidgetTree->FindWidget("MH_White"));
		
		if (Container)
		{
			Container->AddChild(ButtonWidget);
		}
	}
	else
	{
		UVerticalBox* Container = Cast<UVerticalBox>(Cpc->UserInterfaceWidget->WidgetTree->FindWidget("MH_Red"));

		if (Container)
		{
			Container->AddChild(ButtonWidget);
		}
	}
}

/*
 *	@brief	Creates the correct button to insert into the MoveHistory or the EndGame
 *			
 *	@return Void
 */
void AManager_Turn::DisplayEndGame() const
{
	AChess_PlayerController* Cpc = Cast<AChess_PlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	UVerticalBox* Container = Cast<UVerticalBox>(Cpc->UserInterfaceWidget->WidgetTree->FindWidget("EndGame"));

	if (Container)
	{
		UUserWidget* LastButton = Cpc->UserInterfaceWidget->WidgetTree->ConstructWidget<UUserWidget>(EndButtonClass);
		Container->AddChild(LastButton);
	}
}

/*
 *	@brief	Linked to the text above the button via blueprint, it creates the correct notation
 *			to visualize the move just made
 *			
 *	@return String that makes the text in the widget
 */
FString AManager_Turn::ComputeNotation() const
{
	AChess_GameMode* GameMode = Cast<AChess_GameMode>(GetWorld()->GetAuthGameMode());

	FString MoveNomenclature = "";
	if (NextTile && MovedPiece)
	{
		if (bIsCastleLong)
		{
			MoveNomenclature = "O-O-O";
		}
		else if (bIsCastleShort)
		{
			MoveNomenclature = "O-O";
		}
		else
		{
			if (MovedPiece->GetType() == EPieceType::PAWN)
			{
				if (PreviousTile && GameMode->TurnManager->bIsKill)
				{
					MoveNomenclature = FString::Printf(TEXT("%cx"), PreviousTile->GetAlgebraicPosition().TileLetter);
				}
				if (GameMode->TurnManager->bIsPromotion)
				{
					MoveNomenclature = MoveNomenclature + FString::Printf(TEXT("%c%d=%c"), NextTile->GetAlgebraicPosition().TileLetter, NextTile->GetAlgebraicPosition().TileNumber, PromotedPiece->GetNomenclature());
				}
				else
				{
					MoveNomenclature = MoveNomenclature + FString::Printf(TEXT("%c%d"), NextTile->GetAlgebraicPosition().TileLetter, NextTile->GetAlgebraicPosition().TileNumber);
				}
			}
			else
			{
				MoveNomenclature = FString::Printf(TEXT("%c"), MovedPiece->GetNomenclature());
				if (GameMode->TurnManager->bIsKill)
				{
					MoveNomenclature = MoveNomenclature + "x";
				}
				if (MovedPiece->GetType() == EPieceType::ROOK)
				{
					TArray<AChess_Piece*> Team = (MovedPiece->GetTeam() == ETeam::WHITE) ? GameMode->WhiteTeam : GameMode->BlackTeam;
				
					for (AChess_Piece* Piece: Team)
					{
						if (Piece->GetType() == EPieceType::ROOK && Piece != MovedPiece)
						{
							NextTile->SetTileStatus(ETileStatus::EMPTY);
							TArray<ATile*> Moves = Piece->GetLegitMoves();
							NextTile->SetTileStatus(ETileStatus::OCCUPIED);
							if (Moves.Contains(NextTile))
							{
								if (PreviousTile->GetAlgebraicPosition().TileLetter == Piece->GetPieceTile()->GetAlgebraicPosition().TileLetter)
								{
									MoveNomenclature = MoveNomenclature + FString::Printf(TEXT("%i"), PreviousTile->GetAlgebraicPosition().TileNumber);
								}
								else
								{
									MoveNomenclature = MoveNomenclature + FString::Printf(TEXT("%c"), PreviousTile->GetAlgebraicPosition().TileLetter);
								}
							}
						}
					}
				}
				if (MovedPiece->GetType() == EPieceType::KNIGHT)
				{
					TArray<AChess_Piece*> Team = (MovedPiece->GetTeam() == ETeam::WHITE) ? GameMode->WhiteTeam : GameMode->BlackTeam;
				
					for (AChess_Piece* Piece: Team)
					{
						if (Piece->GetType() == EPieceType::KNIGHT && Piece != MovedPiece)
						{
							NextTile->SetTileStatus(ETileStatus::EMPTY);
							TArray<ATile*> Moves = Piece->GetLegitMoves();
							NextTile->SetTileStatus(ETileStatus::OCCUPIED);
							if (Moves.Contains(NextTile))
							{
								if (PreviousTile->GetAlgebraicPosition().TileLetter == Piece->GetPieceTile()->GetAlgebraicPosition().TileLetter)
								{
									MoveNomenclature = MoveNomenclature + FString::Printf(TEXT("%i"), PreviousTile->GetAlgebraicPosition().TileNumber);
								}
								else
								{
									MoveNomenclature = MoveNomenclature + FString::Printf(TEXT("%c"), PreviousTile->GetAlgebraicPosition().TileLetter);
								}
							}
						}
					}
				}
				MoveNomenclature = MoveNomenclature + FString::Printf(TEXT("%c%d"), NextTile->GetAlgebraicPosition().TileLetter, NextTile->GetAlgebraicPosition().TileNumber);
			}
		}
	}
	if (GameMode->bIsWhiteKingInCheckMate || GameMode->bIsBlackKingInCheckMate)
	{
		MoveNomenclature = MoveNomenclature + "#";
	}
	else if (GameMode->TurnManager->bIsWhiteKingInCheck || GameMode->TurnManager->bIsBlackKingInCheck)
	{
		MoveNomenclature = MoveNomenclature + "+";
	}
	return MoveNomenclature;
}

/*
 *	@brief	Disables replay (normally during the AI turn)
 *			
 *	@return Void
 */
void AManager_Turn::DisableReplay() const
{
	AChess_PlayerController* Cpc = Cast<AChess_PlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	UButton* BackButton = Cast<UButton>(Cpc->UserInterfaceWidget->WidgetTree->FindWidget("Back"));
	UButton* ResetButton = Cast<UButton>(Cpc->UserInterfaceWidget->WidgetTree->FindWidget("Reset_Chessboard"));
	if (BackButton)
	{
		BackButton->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
	if (ResetButton)
	{
		ResetButton->SetVisibility(ESlateVisibility::HitTestInvisible);
	}

	for (FMoveInfo Info: MoveHistory)
	{
		Info.Button->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
}

/*
 *	@brief	Enables replay (normally during the Human turn)
 *			
 *	@return Void
 */
void AManager_Turn::EnableReplay()
{
	AChess_PlayerController* Cpc = Cast<AChess_PlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	if (Cpc->UserInterfaceWidget)
	{
		UButton* BackButton = Cast<UButton>(Cpc->UserInterfaceWidget->WidgetTree->FindWidget("Back"));
		UButton* ResetButton = Cast<UButton>(Cpc->UserInterfaceWidget->WidgetTree->FindWidget("Reset_Chessboard"));
		if (BackButton)
		{
			BackButton->SetVisibility(ESlateVisibility::Visible);
		}
		if (ResetButton)
		{
			ResetButton->SetVisibility(ESlateVisibility::Visible);
		}
	}

	UChess_GameInstance* GameInstance = Cast<UChess_GameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (!GameInstance->bNoReplay)
	{
		for (FMoveInfo Info: MoveHistory)
		{
			Info.Button->SetVisibility(ESlateVisibility::Visible);
		}
	}
	else
	{
		for (FMoveInfo Info: MoveHistory)
		{
			Info.Button->SetVisibility(ESlateVisibility::HitTestInvisible);
		}
	}
}

/*
 *	@brief	Destroys all the buttons in the MoveHistory
 *			
 *	@return Void
 */
void AManager_Turn::DestroyMoveHistory()
{
	AChess_PlayerController* Cpc = Cast<AChess_PlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	UVerticalBox* WhiteHistory = Cast<UVerticalBox>(Cpc->UserInterfaceWidget->WidgetTree->FindWidget("MH_White"));
	UVerticalBox* RedHistory = Cast<UVerticalBox>(Cpc->UserInterfaceWidget->WidgetTree->FindWidget("MH_Red"));
	UVerticalBox* EndButton = Cast<UVerticalBox>(Cpc->UserInterfaceWidget->WidgetTree->FindWidget("EndGame"));

	if (WhiteHistory && RedHistory && EndButton)
	{
		WhiteHistory->ClearChildren();
		RedHistory->ClearChildren();
		EndButton->ClearChildren();
	}

	MoveHistory.Empty();
}

/*
 *	@brief	Returns the state of the board to the previous point,
 *			relating to the move and therefore to the button clicked by the user
 *
 *	@param	ClickedIndex: index of the button clicked by the user
 *			
 *	@return Void
 */
void AManager_Turn::Replay(const int32 ClickedIndex)
{
	AChess_GameMode* GameMode = Cast<AChess_GameMode>(GetWorld()->GetAuthGameMode());
	ResetTargetedAndKillableTiles();
	ResetSelectedPiece();
	if (ClickedIndex >= 0 && ClickedIndex < CurrentButtonIndex)
	{
		int32 i = CurrentButtonIndex;
		while (i > 0 && i > ClickedIndex)
		{
			MoveHistory[i].MovedPiece->MovePiece(MoveHistory[i].PreviousTile);

			// Manage kill
			if (MoveHistory[i].KilledPiece)
			{
				MoveHistory[i].KilledPiece->SetActorEnableCollision(true);
				MoveHistory[i].KilledPiece->SetActorHiddenInGame(false);
				MoveHistory[i].NextTile->SetPieceOnTile(MoveHistory[i].KilledPiece);
				MoveHistory[i].NextTile->SetTileStatus(ETileStatus::OCCUPIED);
				MoveHistory[i].NextTile->SetTileTeam(MoveHistory[i].KilledPiece->GetTeam());
				if (MoveHistory[i].KilledPiece->GetTeam() == WHITE)
				{
					GameMode->WhiteTeam.Add(MoveHistory[i].KilledPiece);
				}
				else
				{
					GameMode->BlackTeam.Add(MoveHistory[i].KilledPiece);
				}
			}

			// Manage promotion
			if (MoveHistory[i].PromotedPiece)
			{
				MoveHistory[i].PromotedPiece->SetActorHiddenInGame(true);
				MoveHistory[i].PromotedPiece->SetActorEnableCollision(false);

				MoveHistory[i].MovedPiece->SetActorHiddenInGame(false);
				MoveHistory[i].MovedPiece->SetActorEnableCollision(true);

				if (MoveHistory[i].PromotedPiece->GetTeam() == ETeam::WHITE)
				{
					GameMode->WhiteTeam.Remove(MoveHistory[i].PromotedPiece);
					GameMode->WhiteTeam.Add(MoveHistory[i].MovedPiece);
				}
				else
				{
					GameMode->BlackTeam.Remove(MoveHistory[i].PromotedPiece);
					GameMode->BlackTeam.Add(MoveHistory[i].MovedPiece);
				}
			}

			// Manage castle
			if (MoveHistory[i].RookCastle && MoveHistory[i].RookPreviousTile)
			{
				MoveHistory[i].RookCastle->MovePiece(MoveHistory[i].RookPreviousTile);
				MoveHistory[i].RookCastle->bAlreadyMoved = false;
			}

			// Manage first move
			if (MoveHistory[i].bJustMoved) 
			{
				MoveHistory[i].MovedPiece->bAlreadyMoved = false;
			}
			GameMode->TurnManager->ResetVariables();
			GameMode->GetAllLegalMoves(i%2);
			i--;
		}
	}
	if (ClickedIndex <= MoveHistory.Num() - 1 && ClickedIndex > CurrentButtonIndex)
	{
		int32 i = CurrentButtonIndex;
		while (i <= MoveHistory.Num() - 1 && i <= ClickedIndex)
		{
			if (MoveHistory[i].PromotedPiece)
			{
				MoveHistory[i].MovedPiece->SetActorHiddenInGame(true);
				MoveHistory[i].MovedPiece->SetActorEnableCollision(false);
				MoveHistory[i].PromotedPiece->SetActorHiddenInGame(false);
				MoveHistory[i].PromotedPiece->SetActorEnableCollision(true);
				
				if (MoveHistory[i].PromotedPiece->GetTeam() == ETeam::WHITE)
				{
					GameMode->WhiteTeam.Remove(MoveHistory[i].MovedPiece);
					GameMode->WhiteTeam.Add(MoveHistory[i].PromotedPiece);
				}
				else
				{
					GameMode->BlackTeam.Remove(MoveHistory[i].MovedPiece);
					GameMode->BlackTeam.Add(MoveHistory[i].PromotedPiece);
				}
			}
			
			if (MoveHistory[i].KilledPiece)
			{
				if (MoveHistory[i].KilledPiece->GetTeam() == ETeam::WHITE)
				{
					GameMode->WhiteTeam.Remove(MoveHistory[i].KilledPiece);
				}
				else
				{
					GameMode->BlackTeam.Remove(MoveHistory[i].KilledPiece);
				}
				MoveHistory[i].KilledPiece->SetActorHiddenInGame(true);
				MoveHistory[i].KilledPiece->SetActorEnableCollision(false);
			}

			// Manage castle
			if (MoveHistory[i].RookCastle && MoveHistory[i].RookPreviousTile)
			{
				int32 PreviousTileIndex = GameMode->TileArray.Find(MoveHistory[i].RookPreviousTile);
				int32 KingIndex = GameMode->TileArray.Find(MoveHistory[i].NextTile);
				if (PreviousTileIndex > KingIndex)
				{
					MoveHistory[i].RookCastle->MovePiece(GameMode->TileArray[KingIndex-1]);
				}
				else if (PreviousTileIndex < KingIndex)
				{
					MoveHistory[i].RookCastle->MovePiece(GameMode->TileArray[KingIndex+1]);
				}
			}

			/*
			if (MoveHistory[i].bJustMoved) 
			{
				MoveHistory[i].MovedPiece->bAlreadyMoved = true;
			}
			*/
			
			MoveHistory[i].MovedPiece->MovePiece(MoveHistory[i].NextTile);
			GameMode->TurnManager->ResetVariables();
			GameMode->GetAllLegalMoves(i%2);
			i++;
		}
	}

	/*
	// I calculate the legal moves only for white (Human Player) only when
	// it is his turn, and therefore he can execute the move
	if ((ClickedIndex % 2) != 0)
	{
		GameMode->GetAllLegalMoves(0);
	}
	*/
	GameMode->UpdateScores();
	CurrentButtonIndex = ClickedIndex;
}

// Called when the game starts or when spawned
void AManager_Turn::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AManager_Turn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

