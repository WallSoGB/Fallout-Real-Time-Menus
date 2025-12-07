#pragma once

class MagicSystem {
public:
	enum SpellType : uint32_t {
		SPELL,
		DISEASE,
		POWER,
		LESSER_POWER,
		ABILITY,
		POISON,
		ENCHANTMENT,
		POTION,
		WORTCRAFT,
		LEVELLED,
		ADDICTION,
	};

	enum Range : uint32_t {
		SELF,
		TOUCH,
		TARGET,
		ANY,
	};

	enum School : uint32_t {
		ALTERATION,
		CONJURATION,
		DESTRUCTION,
		ILLUSION,
		MYSTICISM,
		RESTORATION,
	};
};