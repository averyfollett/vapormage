// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once 

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "GrilledPigeonHUD.generated.h"

UCLASS()
class AGrilledPigeonHUD : public AHUD
{
	GENERATED_BODY()

public:
	AGrilledPigeonHUD();

	/** Primary draw call for the HUD */
	virtual void DrawHUD() override;

private:
	/** Crosshair asset pointer */
	class UTexture2D* CrosshairTex;

};

