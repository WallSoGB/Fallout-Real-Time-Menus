#pragma once

class CharacterProgressionData {
public:
	bool		bShouldLevelUp;
	uint32_t	uiXPForNextLevel;
	bool		bIsChangingMaxXP;
};

ASSERT_SIZE(CharacterProgressionData, 0xC);

class CharacterProgression {
public:
	CharacterProgressionData* pData;
};

ASSERT_SIZE(CharacterProgression, 0x4);