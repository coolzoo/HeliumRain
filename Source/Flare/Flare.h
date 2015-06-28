
#ifndef __FLARE_H__
#define __FLARE_H__

#include "Engine.h"
#include "UI/Style/FlareStyleSet.h"


/*----------------------------------------------------
	UI enums
----------------------------------------------------*/

/** Possible menu targets */
UENUM()
namespace EFlareMenu
{
	enum Type
	{
		MENU_None,
		MENU_Main,
		MENU_Dashboard,
		MENU_Company,
		MENU_FlyShip,
		MENU_Ship,
		MENU_ShipConfig,
		MENU_Station,
		MENU_Undock,
		MENU_Sector,
		MENU_Settings,
		MENU_Quit,
		MENU_Exit
	};
}


/*----------------------------------------------------
	Helpers
----------------------------------------------------*/

#define NULL_QUAT           (FQuat::MakeFromEuler(FVector::ZeroVector))

#define FLOG(Format)        UE_LOG(LogTemp, Display, TEXT(Format))
#define FLOGV(Format, ...)  UE_LOG(LogTemp, Display, TEXT(Format), __VA_ARGS__)

DECLARE_LOG_CATEGORY_EXTERN(LogFlare, Log, All);


/*----------------------------------------------------
	Game module definition
----------------------------------------------------*/

class FFlareModule : public FDefaultGameModuleImpl
{

public:

	void StartupModule() override;

	void ShutdownModule() override;


protected:

	FFlareStyleSet StyleInstance;

};


#endif
