// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class CHESS_API Manager_Thread: public FRunnable
{
public:
	Manager_Thread(UWorld* InWorld);
	virtual ~Manager_Thread() override;

private:
	bool bStop;
	UWorld* World;
	FRunnableThread* Thread;

public:
	virtual void Exit() override;
	virtual bool Init() override;
	virtual uint32 Run() override;
	virtual void Stop() override;
};
