#pragma once

class TESObjectREFR;

class BGSOpenCloseForm {
public:
	virtual void	HandleOpenFinish(TESObjectREFR* apItemActivated, TESObjectREFR* apActionRef);
	virtual void	HandleCloseFinish(TESObjectREFR* apItemActivated, TESObjectREFR* apActionRef);
	virtual bool	GetBlockActivatesWhileOpening() const;

	BGSOpenCloseForm();
	~BGSOpenCloseForm();

	struct _OpenCloseState {
		enum State : uint32_t {
			NONE	= 0,
			OPEN	= 1,
			OPENING = 2,
			CLOSED	= 3,
			CLOSING = 4,
		};
	};
	using OpenCloseState = _OpenCloseState::State;

	static void HandleActivate(TESObjectREFR* apItemActivated, TESObjectREFR* apActionRef, BGSOpenCloseForm* apDoor = nullptr);

	static OpenCloseState GetOpenState(TESObjectREFR* apRef);
};

ASSERT_SIZE(BGSOpenCloseForm, 0x4);