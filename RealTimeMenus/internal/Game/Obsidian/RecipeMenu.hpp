#pragma once

#include "Bethesda/Menu.hpp"
#include "Bethesda/ListBox.hpp"

class TESObjectREFR;
class TESRecipeCategory;
class TESRecipe;
class TESRecipeComponent;
class TESConditionItem;

class RecipeMenu : public Menu {
public:
	Tile*							pTiles[14];
	TESObjectREFR*					pReference;
	TESRecipeCategory*				pCategory;
	uint32_t						uiMenuSoundID;
	ListBox<TESRecipe*>				kRecipeList;
	ListBox<TESRecipe*>*			pCurrentItems;
	ListBox<TESRecipeComponent*>	kComponentList;
	ListBox<TESConditionItem*>		kConditionList;
	bool							bTutorialShown;

	static RecipeMenu* GetSingleton();
};

ASSERT_SIZE(RecipeMenu, 0x104);