#pragma once

#include "TESPackage.hpp"
#include "BSSoundHandle.hpp"

class Conversation;
class DialogueItem;
class DialogueResponse;
class Actor;
class MobileObject;
class TESTopic;

class DialoguePackage : public TESPackage {
public:
	DialoguePackage();
	~DialoguePackage();

	BSSoundHandle		kSound;
	TESTopic*			pTopic;
	float				fListenTimer;
	Actor*				pTalkingActor;
	bool				bWaitingforLipFile;
	TESForm*			pTargetLocation;
	bool				bHasIdles;
	Conversation*		pConversation;
	DialogueItem*		pCurrentItem;
	DialogueResponse*	pCurrentResponse;
	Actor*				pActor;
	Actor*				pTargetActor;
	MobileObject*		pSpeaker_0B8;
	uint8_t				unk0BC;
	bool				bCreated;
	bool				bCanMove;
	uint8_t				unk0BF;
	uint8_t				unk0C0;
	uint8_t				unk0C1;
	uint8_t				unk0C2;
	uint8_t				unk0C3;
	uint32_t			uiResponseEmotionType;
	uint32_t			uiResponseEmotionValue;
	uint8_t				unk0CC;
};

ASSERT_SIZE(DialoguePackage, 0xD0);