// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class CHESS_API AsyncMinimax: public FRunnable
{
public:
	AsyncMinimax(UWorld* InWorld);
	virtual ~AsyncMinimax() override;

private:
	bool bStop;
	UWorld* World;

public:
	virtual void Exit() override;
	virtual bool Init() override;
	virtual uint32 Run() override;
	virtual void Stop() override;
};
