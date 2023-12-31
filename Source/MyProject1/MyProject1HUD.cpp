// Fill out your copyright notice in the Description page of Project Settings.


#include "MyProject1HUD.h"
//#include "WheeledVehicle.h"
#include "RenderResource.h"
#include "Shader.h"
#include "Engine/Canvas.h"
//#include "WheeledVehicleMovementComponent.h"
#include "Engine/Font.h"
#include "CanvasItem.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/Engine.h"

// Needed for VR Headset
#if HMD_MODULE_INCLUDED
#include "IHeadMountedDisplay.h"
#endif // HMD_MODULE_INCLUDED 

#define LOCTEXT_NAMESPACE "VehicleHUD"

AMyProject1HUD::AMyProject1HUD()
{
	static ConstructorHelpers::FObjectFinder<UFont> Font(TEXT("/Engine/EngineFonts/RobotoDistanceField"));
	HUDFont = Font.Object;
}

void AMyProject1HUD::DrawHUD()
{
	Super::DrawHUD();

	// Calculate ratio from 720p
	const float HUDXRatio = Canvas->SizeX / 1280.f;
	const float HUDYRatio = Canvas->SizeY / 720.f;

	bool bWantHUD = true;
#if HMD_MODULE_INCLUDED
	//	if (GEngine->XRSystem.IsValid() && GEngine->XRSystem->GetSystemName() != TEXT("None"))
	//	{
	//		bWantHUD = GEngine->XRSystem->IsStereoEnabled();
	//	}
#endif // HMD_MODULE_INCLUDED
	// We dont want the onscreen hud when using a HMD device	
//	if (bWantHUD == true)
//	{
		// Get our vehicle so we can check if we are in car. If we are we don't want onscreen HUD
//		* Vehicle = Cast<AKrazyKartsPawn>(GetOwningPawn());
//		if ((Vehicle != nullptr) && (Vehicle->bInCarCameraActive == false))
//		{
//			FVector2D ScaleVec(HUDYRatio * 1.4f, HUDYRatio * 1.4f);
//
//			// Speed
//			FCanvasTextItem SpeedTextItem(FVector2D(HUDXRatio * 805.f, HUDYRatio * 455), Vehicle->SpeedDisplayString, HUDFont, FLinearColor::White);
//			SpeedTextItem.Scale = ScaleVec;
//			Canvas->DrawItem(SpeedTextItem);
//
//			// Gear
//			FCanvasTextItem GearTextItem(FVector2D(HUDXRatio * 805.f, HUDYRatio * 500.f), Vehicle->GearDisplayString, HUDFont, Vehicle->bInReverseGear == false ? Vehicle->GearDisplayColor : Vehicle->GearDisplayReverseColor);
//			GearTextItem.Scale = ScaleVec;
//			Canvas->DrawItem(GearTextItem);
//		}
//	}
}


#undef LOCTEXT_NAMESPACE
