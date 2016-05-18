
#include "../../Flare.h"
#include "FlareResourcePricesMenu.h"
#include "FlareWorldEconomyMenu.h"
#include "../../Game/FlareGame.h"
#include "../../Economy/FlareResource.h"
#include "../../Player/FlareMenuManager.h"
#include "../../Player/FlarePlayerController.h"


#define LOCTEXT_NAMESPACE "FlareResourcePricesMenu"


/*----------------------------------------------------
	Construct
----------------------------------------------------*/

void SFlareResourcePricesMenu::Construct(const FArguments& InArgs)
{
	MenuManager = InArgs._MenuManager;
	const FFlareStyleCatalog& Theme = FFlareStyleSet::GetDefaultTheme();

	// Build structure
	ChildSlot
	.HAlign(HAlign_Fill)
	.VAlign(VAlign_Fill)
	[
		SNew(SVerticalBox)

		// Title
		+ SVerticalBox::Slot()
		.AutoHeight()
		.HAlign(HAlign_Fill)
		.Padding(Theme.ContentPadding)
		[
			SNew(SHorizontalBox)

			// Icon
			+ SHorizontalBox::Slot()
			.AutoWidth()
			[
				SNew(SImage).Image(AFlareMenuManager::GetMenuIcon(EFlareMenu::MENU_ResourcePrices))
			]

			// Title
			+ SHorizontalBox::Slot()
			.VAlign(VAlign_Center)
			.Padding(Theme.ContentPadding)
			[
				SNew(STextBlock)
				.Text(LOCTEXT("ResourcePricesTitle", "RESOURCE PRICES"))
				.TextStyle(&Theme.TitleFont)
			]

			// Close
			+ SHorizontalBox::Slot()
			.HAlign(HAlign_Right)
			.VAlign(VAlign_Bottom)
			.Padding(Theme.TitleButtonPadding)
			.AutoWidth()
			[
				SNew(SFlareRoundButton)
				.Text(LOCTEXT("Back", "Back"))
				.HelpText(LOCTEXT("BackInfo", "Go to the previous menu"))
				.Icon(AFlareMenuManager::GetMenuIcon(EFlareMenu::MENU_Exit, true))
				.OnClicked(this, &SFlareResourcePricesMenu::Back)
			]
		]

		// Separator
		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(FMargin(200, 20))
		[
			SNew(SImage).Image(&Theme.SeparatorBrush)
		]

		// List header
		+ SVerticalBox::Slot()
		.HAlign(HAlign_Center)
		.Padding(Theme.ContentPadding)
		.AutoHeight()
		[
			SNew(SBox)
			.WidthOverride(1.7 * Theme.ContentWidth)
			.HAlign(HAlign_Fill)
			.Padding(FMargin(1))
			[

				SNew(SHorizontalBox)

				// Icon space
				+ SHorizontalBox::Slot()
				.AutoWidth()
				[
					SNew(SBox)
					.WidthOverride(Theme.ResourceWidth)
					.Padding(FMargin(0))
				]

				// Info
				+ SHorizontalBox::Slot()
				.AutoWidth()
				.Padding(Theme.ContentPadding)
				[
					SNew(SBox)
					.WidthOverride(0.5 * Theme.ContentWidth)
					.HAlign(HAlign_Left)
					[
						SNew(STextBlock)
						.TextStyle(&Theme.NameFont)
						.Text(LOCTEXT("ResourceInfo", "Resource info"))
					]
				]

				// Prices
				+ SHorizontalBox::Slot()
				.AutoWidth()
				.Padding(Theme.ContentPadding)
				[
					SNew(SBox)
					.WidthOverride(0.2 * Theme.ContentWidth)
					.HAlign(HAlign_Left)
					[
						SNew(STextBlock)
						.TextStyle(&Theme.NameFont)
						.Text(this, &SFlareResourcePricesMenu::GetSectorPriceInfo)
						.WrapTextAt(0.2 * Theme.ContentWidth)
					]
				]

				// Variation
				+ SHorizontalBox::Slot()
				.AutoWidth()
				.Padding(Theme.ContentPadding)
				[
					SNew(SBox)
					.WidthOverride(0.2 * Theme.ContentWidth)
					.HAlign(HAlign_Left)
					[
						SNew(STextBlock)
						.TextStyle(&Theme.NameFont)
						.Text(LOCTEXT("ResourcePriceVariation", "Variation on 40 days"))
						.WrapTextAt(0.2 * Theme.ContentWidth)
					]
				]


				// Transport fee
				+ SHorizontalBox::Slot()
				.AutoWidth()
				.Padding(Theme.ContentPadding)
				[
					SNew(SBox)
					.WidthOverride(0.2 * Theme.ContentWidth)
					.HAlign(HAlign_Left)
					[
						SNew(STextBlock)
						.TextStyle(&Theme.NameFont)
						.Text(LOCTEXT("TransportFee", "Transport fee"))
					]
				]

				// Icon space
				+ SHorizontalBox::Slot()
				.AutoWidth()
				.Padding(Theme.ContentPadding)
				[
					SNew(SBox)
					.WidthOverride(0.3 * Theme.ContentWidth)
					.Padding(FMargin(0))
				]
			]
		]


		// List
		+ SVerticalBox::Slot()
		.HAlign(HAlign_Center)
		.Padding(Theme.ContentPadding)
		[
			SNew(SBox)
			.WidthOverride(1.7 * Theme.ContentWidth)
			[
				SNew(SScrollBox)
				.Style(&Theme.ScrollBoxStyle)
				.ScrollBarStyle(&Theme.ScrollBarStyle)

				+ SScrollBox::Slot()
				[
					SNew(SBox)
					.HAlign(HAlign_Fill)
					[
						SAssignNew(ResourcePriceList, SVerticalBox)
					]
				]
			]
		]
	];
}


/*----------------------------------------------------
	Interaction
----------------------------------------------------*/

void SFlareResourcePricesMenu::Setup()
{
	SetEnabled(false);
	SetVisibility(EVisibility::Collapsed);
}

void SFlareResourcePricesMenu::Enter(UFlareSectorInterface* Sector)
{
	FLOG("SFlareResourcePricesMenu::Enter");
	SetEnabled(true);
	SetVisibility(EVisibility::Visible);
	MenuManager->UseDarkBackground();

	TargetSector = Sector;

	// Resource prices
	ResourcePriceList->ClearChildren();
	const FFlareStyleCatalog& Theme = FFlareStyleSet::GetDefaultTheme();
	TArray<UFlareResourceCatalogEntry*> ResourceList = MenuManager->GetGame()->GetResourceCatalog()->GetResourceList();
	ResourceList.Sort(&SortByResourceType);

	// Resource prices
	for (int32 ResourceIndex = 0; ResourceIndex < ResourceList.Num(); ResourceIndex++)
	{
		FFlareResourceDescription& Resource = ResourceList[ResourceIndex]->Data;
		ResourcePriceList->AddSlot()
		.Padding(FMargin(1))
		[
			SNew(SBorder)
			.Padding(FMargin(1))
			.BorderImage((ResourceIndex % 2 == 0 ? &Theme.EvenBrush : &Theme.OddBrush))
			[
				SNew(SHorizontalBox)

				// Icon
				+ SHorizontalBox::Slot()
				.AutoWidth()
				[
					SNew(SBorder)
					.Padding(FMargin(0))
					.BorderImage(&Resource.Icon)
					[
						SNew(SBox)
						.WidthOverride(Theme.ResourceWidth)
						.HeightOverride(Theme.ResourceHeight)
						.Padding(FMargin(0))
						[
							SNew(STextBlock)
							.TextStyle(&Theme.TextFont)
							.Text(Resource.Acronym)
						]
					]
				]

				// Info
				+ SHorizontalBox::Slot()
				.AutoWidth()
				.VAlign(VAlign_Center)
				.Padding(Theme.ContentPadding)
				[
					SNew(SBox)
					.WidthOverride(0.5 * Theme.ContentWidth)
					.HAlign(HAlign_Left)
					[
						SNew(STextBlock)
						.TextStyle(&Theme.TextFont)
						.Text(Resource.Description)
						.WrapTextAt(0.5 * Theme.ContentWidth)
					]
				]

				// Price
				+ SHorizontalBox::Slot()
				.AutoWidth()
				.VAlign(VAlign_Center)
				.Padding(Theme.ContentPadding)
				[
					SNew(SBox)
					.WidthOverride(0.2 * Theme.ContentWidth)
					.HAlign(HAlign_Left)
					[
						SNew(STextBlock)
						.TextStyle(&Theme.TextFont)
						.ColorAndOpacity(this, &SFlareResourcePricesMenu::GetPriceColor, &Resource)
						.Text(this, &SFlareResourcePricesMenu::GetResourcePriceInfo, &Resource)
					]
				]

				// Price variation
				+ SHorizontalBox::Slot()
				.AutoWidth()
				.VAlign(VAlign_Center)
				.Padding(Theme.ContentPadding)
				[
					SNew(SBox)
					.WidthOverride(0.2 * Theme.ContentWidth)
					.HAlign(HAlign_Left)
					[
						SNew(STextBlock)
						.TextStyle(&Theme.TextFont)
						.Text(this, &SFlareResourcePricesMenu::GetResourcePriceVariationInfo, &Resource)
					]
				]

				// Transport fee
				+ SHorizontalBox::Slot()
				.AutoWidth()
				.VAlign(VAlign_Center)
				.Padding(Theme.ContentPadding)
				[
					SNew(SBox)
					.WidthOverride(0.2 * Theme.ContentWidth)
					.HAlign(HAlign_Left)
					[
						SNew(STextBlock)
						.TextStyle(&Theme.TextFont)
						.Text(this, &SFlareResourcePricesMenu::GetResourceTransportFeeInfo, &Resource)
					]
				]

				// Details
				+ SHorizontalBox::Slot()
				.AutoWidth()
				.VAlign(VAlign_Center)
				.Padding(Theme.ContentPadding)
				[
					SNew(SBox)
					.WidthOverride(0.2 * Theme.ContentWidth)
					.HAlign(HAlign_Left)
					[
						SNew(SFlareButton)
						.Text(LOCTEXT("DetailButton", "Details"))
						.OnClicked(this, &SFlareResourcePricesMenu::OnShowWorldInfosClicked, &Resource)
						.Icon(FFlareStyleSet::GetIcon("Travel"))
					]
				]
			]
		];
	}
}

void SFlareResourcePricesMenu::Exit()
{
	SetEnabled(false);
	SetVisibility(EVisibility::Collapsed);
	ResourcePriceList->ClearChildren();
	TargetSector = NULL;
}

void SFlareResourcePricesMenu::Back()
{
	if (TargetSector)
	{
		MenuManager->OpenMenu(EFlareMenu::MENU_Sector, TargetSector);
	}
	else
	{
		MenuManager->OpenMenu(EFlareMenu::MENU_Orbit);
	}
}


/*----------------------------------------------------
	Callbacks
----------------------------------------------------*/

FSlateColor SFlareResourcePricesMenu::GetPriceColor(FFlareResourceDescription* Resource) const
{
	const FFlareStyleCatalog& Theme = FFlareStyleSet::GetDefaultTheme();
	if (TargetSector)
	{
		FLinearColor HightPriceColor = Theme.FriendlyColor;
		FLinearColor LowPriceColor = Theme.EnemyColor;

		float ResourcePrice = TargetSector->GetPreciseResourcePrice(Resource);

		float PriceRatio = (ResourcePrice - Resource->MinPrice) / (float) (Resource->MaxPrice - Resource->MinPrice);


		return FMath::Lerp(LowPriceColor, HightPriceColor, PriceRatio);
	}
	return Theme.FriendlyColor;
}

void SFlareResourcePricesMenu::OnShowWorldInfosClicked(FFlareResourceDescription* Resource)
{
	FFlareWorldEconomyMenuParam* Params = new FFlareWorldEconomyMenuParam();
	Params->Resource = Resource;
	Params->Sector = TargetSector;

	MenuManager->OpenMenu(EFlareMenu::MENU_WorldEconomy, Params);
}

FText SFlareResourcePricesMenu::GetSectorPriceInfo() const
{
	FText Text;

	if (TargetSector)
	{
		Text = FText::Format(LOCTEXT("ResourcePrice", "Prices in {0}"), TargetSector->GetSectorName());
	}

	return Text;
}


FText SFlareResourcePricesMenu::GetResourcePriceInfo(FFlareResourceDescription* Resource) const
{
	if (TargetSector)
	{
		FNumberFormattingOptions MoneyFormat;
		MoneyFormat.MaximumFractionalDigits = 2;

		int64 ResourcePrice = TargetSector->GetResourcePrice(Resource, EFlareResourcePriceContext::Default);

		return FText::Format(LOCTEXT("ResourceMainPriceFormat", "{0} credits"),
			FText::AsNumber(ResourcePrice / 100.0f, &MoneyFormat));
	}

	return FText();
}

FText SFlareResourcePricesMenu::GetResourcePriceVariationInfo(FFlareResourceDescription* Resource) const
{
	if (TargetSector)
	{
		FNumberFormattingOptions MoneyFormat;
		MoneyFormat.MaximumFractionalDigits = 2;

		int32 MeanDuration = 40;
		int64 ResourcePrice = TargetSector->GetResourcePrice(Resource, EFlareResourcePriceContext::Default);
		int64 LastResourcePrice = TargetSector->GetResourcePrice(Resource, EFlareResourcePriceContext::Default, MeanDuration);

		if(ResourcePrice != LastResourcePrice)
		{
			float Variation = (((float) ResourcePrice) / ((float) LastResourcePrice) - 1);

			if(FMath::Abs(Variation) >= 0.0001)
			{
				return FText::Format(LOCTEXT("ResourceVariationFormat", "{0}{1}%"),
								(Variation > 0 ?
									 FText::FText(LOCTEXT("ResourceVariationFormatSignPlus","+")) :
									 FText::FText(LOCTEXT("ResourceVariationFormatSignMinus","-"))),
							  FText::AsNumber(FMath::Abs(Variation) * 100.0f, &MoneyFormat));
			}
		}

		return FText::FText(LOCTEXT("ResourceMainPriceNoVariationFormat", "-"));
	}

	return FText();
}

FText SFlareResourcePricesMenu::GetResourceTransportFeeInfo(FFlareResourceDescription* Resource) const
{
	if (TargetSector)
	{
		FNumberFormattingOptions MoneyFormat;
		MoneyFormat.MaximumFractionalDigits = 2;

		return FText::Format(LOCTEXT("ResourceMainPriceFormat", "{0} credits"),
			FText::AsNumber(Resource->TransportFee / 100.0f, &MoneyFormat));
	}

	return FText();
}

#undef LOCTEXT_NAMESPACE

