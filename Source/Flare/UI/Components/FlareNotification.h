#pragma once

#include "../../Flare.h"
#include "FlareNotification.generated.h"


class AFlareMenuManager;


/** Possible notification types */
UENUM()
namespace EFlareNotification
{
	enum Type
	{
		NT_General,
		NT_Help,
		NT_Trading,
		NT_Military,
		NT_Quest
	};
}


class SFlareNotification : public SCompoundWidget
{
	/*----------------------------------------------------
		Slate arguments
	----------------------------------------------------*/

	SLATE_BEGIN_ARGS(SFlareNotification)
	{}

	SLATE_ARGUMENT(AFlareMenuManager*, MenuManager)
	SLATE_ARGUMENT(EFlareNotification::Type, Type)
	SLATE_ARGUMENT(FText, Text)
	SLATE_ARGUMENT(FText, Info)
	SLATE_ARGUMENT(float, Timeout)
	SLATE_ARGUMENT(EFlareMenu::Type, TargetMenu)
	SLATE_ARGUMENT(void*, TargetInfo)

	SLATE_END_ARGS()


public:

	/*----------------------------------------------------
		Public methods
	----------------------------------------------------*/

	/** Create the widget */
	void Construct(const FArguments& InArgs);

	/** Can we safely delete this ? */
	bool IsFinished() const;

	/** Check if this notification is similar to... */
	bool IsDuplicate(const FText& OtherText, const EFlareMenu::Type OtherMenu) const;

	/** Complete this notification */
	void Finish(bool Now = true);

	
	/*----------------------------------------------------
		Callbacks
	----------------------------------------------------*/
	
	void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override;
	
	/** Get the current color */
	FSlateColor GetNotificationColor(EFlareNotification::Type Type) const;

	/** Get the current text color */
	FSlateColor GetNotificationTextColor() const;

	/** Get the current background color */
	FSlateColor GetNotificationBackgroundColor() const;

	/** Get the margins */
	FMargin GetNotificationMargins() const;

	/** We clicked something */
	FReply OnNotificationClicked();

	

protected:

	/*----------------------------------------------------
		Protected data
	----------------------------------------------------*/

	// Settings
	float                                NotificationFinishDuration;
	float                                NotificationTimeout;
	float                                NotificationScroll;
	float                                NotificationEnterDuration;
	float                                NotificationExitDuration;

	/** HUD reference */
	UPROPERTY()
	TWeakObjectPtr<class AFlareMenuManager>      MenuManager;

	// Gameplay
	float                                Lifetime;
	TEnumAsByte<EFlareMenu::Type>        TargetMenu;
	void*                                TargetInfo;
	FText                                Text;

	// Fade data
	TSharedPtr<SButton>                  Button;
	float                                CurrentAlpha;
	float                                CurrentMargin;
	float                                LastHeight;


};
