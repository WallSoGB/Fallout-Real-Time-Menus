#pragma once

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

class Controls {
private:
	LPCDIDATAFORMAT GetMouseDataFormat();
	LPCDIDATAFORMAT GetKeyboardDataFormat();

public:
	enum Scancodes {
		_Escape = 0x1,
		One = 0x2,
		Two = 0x3,
		Three = 0x4,
		Four = 0x5,
		Five = 0x6,
		Six = 0x7,
		Seven = 0x8,
		Eight = 0x9,
		Nine = 0xA,
		Zero = 0xB,
		Minus = 0xC,
		Equals = 0xD,
		Backspace = 0xE,
		Tab = 0xF,
		Q = 0x10,
		W = 0x11,
		E = 0x12,
		R = 0x13,
		T = 0x14,
		Y = 0x15,
		U = 0x16,
		I = 0x17,
		O = 0x18,
		P = 0x19,
		LeftBracket = 0x1A,
		RightBracket = 0x1B,
		Enter = 0x1C,
		LeftControl = 0x1D,
		A = 0x1E,
		S = 0x1F,
		D = 0x20,
		F = 0x21,
		G = 0x22,
		H = 0x23,
		J = 0x24,
		K = 0x25,
		L = 0x26,
		SemiColon = 0x27,
		Apostrophe = 0x28,
		Tilde = 0x29,
		LeftShift = 0x2A,
		BackSlash = 0x2B,
		Z = 0x2C,
		X = 0x2D,
		C = 0x2E,
		V = 0x2F,
		B = 0x30,
		N = 0x31,
		M = 0x32,
		Comma = 0x33,
		Period = 0x34,
		ForwardSlash = 0x35,
		RightShift = 0x36,
		NumStar = 0x37,
		LeftAlt = 0x38,
		SpaceBar = 0x39,
		CapsLock = 0x3A,
		F1 = 0x3B,
		F2 = 0x3C,
		F3 = 0x3D,
		F4 = 0x3E,
		F5 = 0x3F,
		F6 = 0x40,
		F7 = 0x41,
		F8 = 0x42,
		F9 = 0x43,
		F10 = 0x44,
		NumLock = 0x45,
		ScrollLock = 0x46,
		Num7 = 0x47,
		Num8 = 0x48,
		Num9 = 0x49,
		NumMinus = 0x4A,
		Num4 = 0x4B,
		Num5 = 0x4C,
		Num6 = 0x4D,
		NumPlus = 0x4E,
		Num1 = 0x4F,
		Num2 = 0x50,
		Num3 = 0x51,
		Num0 = 0x52,
		NumPeriod = 0x53,
		F11 = 0x57,
		F12 = 0x58,
		NumEnter = 0x9C,
		RightControl = 0x9D,
		NumSlash = 0xB5,
		PrintScrn = 0xB7,
		RightAlt = 0xB8,
		Home = 0xC7,
		UpArrow = 0xC8,
		PageUp = 0xC9,
		LeftArrow = 0xCB,
		RightArrow = 0xCD,
		End_ = 0xCF,
		DownArrow = 0xD0,
		PageDown = 0xD1,
		Delete_ = 0xD3,
	};

	enum MouseScanCodes {
		LeftMouse = 0x0,
		RightMouse = 0x1,
		MiddleMouse = 0x2,
		MouseButton3 = 0x3,
		MouseButton4 = 0x4,
		MouseButton5 = 0x5,
		MouseButton6 = 0x6,
		MouseButton7 = 0x7,
		MouseWheelUp = 0x8,
		MouseWheelDown = 0x9,
		Mouse_MAX = 0xA,
	};

	struct Devices {
		enum {
			KEYBOARD = 0,
			MOUSE,
			JOYSTICK,
			CONTROLLER,
			COUNT
		};
	};

	struct KeyState {
		enum {
			NONE = -1,
			HELD = 0,
			PRESSED = 1,
			DEPRESSED = 2,
			CHANGED = 3,
		};
	};

	enum Flags {
		kFlag_HasJoysticks = 1u << 0,
		kFlag_HasMouse = 1u << 1,
		kFlag_HasKeyboard = 1u << 2,
		kFlag_BackgroundMouse = 1u << 3,
	};

	enum {
		kMaxControlBinds = 0x1C,
	};


	enum {
		kMaxDevices = 8,
	};

	struct JoystickObjectsInfo {
		enum {
			kHasXAxis = 1u << 0,
			kHasYAxis = 1u << 1,
			kHasZAxis = 1u << 2,
			kHasXRotAxis = 1u << 3,
			kHasYRotAxis = 1u << 4,
			kHasZRotAxis = 1u << 5
		};

		uint32_t	uiAxis;
		uint32_t	uiButtons;
	};

	struct VibrationStates {
		uint32_t	uiMotorStrength;
		uint32_t	uiPattern;
		uint32_t	uiDurationMS;
		uint32_t	uiPulseDuration;
		uint8_t		byte10;
		uint32_t	unk14;
		uint16_t	wrd18[2];
		uint32_t	unk1C;
		uint16_t	wrd20[2];
	};


	struct ControllerVibration {
		struct MotorParams {
			uint32_t duration;
			uint32_t unk04;
			uint32_t unk08;
			uint32_t uiTimePassed;
			uint32_t unk10;
		};


		uint32_t			uiControllerIndex;
		VibrationStates*	pLeftStates;
		VibrationStates*	pRightStates;
		MotorParams			LeftParams;
		MotorParams			RightParams;
		uint32_t			unk34;
		uint32_t			uiTickCount;
		uint32_t			unk3C;
	};

	struct DeviceState {
		enum {
			CURRENT = 0,
			PREVIOUS = 1,
			COUNT
		};
	};

	struct MouseAxes {
		enum {
			DELTA_X			= 0,
			DELTA_Y			= 1,
			DELTA_SCROLL	= 2,
		};
	};

	bool					bIsControllerDisabled;
	Bitfield32				uiFlags;
	LPDIRECTINPUT8A			pDirectInput;
	LPDIRECTINPUTDEVICE8A	pJoySticks[8];
	LPDIRECTINPUTDEVICE8A	pKeyboard;
	LPDIRECTINPUTDEVICE8A	pMouse;
	DIJOYSTATE				kJoyStickState[8][DeviceState::COUNT];
	DIDEVICEINSTANCEA		kJoyStickInstances[8];
	DIDEVCAPS				kJoyStickCaps[8];
	uint32_t				uiJoyStickFlags[8][DeviceState::COUNT];
	uint32_t				uiJoyStickCount;
	uint8_t					ucKeyboardState[DeviceState::COUNT][256];
	DIDEVCAPS				kMouseCaps;
	DIMOUSESTATE2			kMouseState[DeviceState::COUNT];
	BOOL					bSwapLeftRightMouseButtons;
	uint8_t					ucMouseSensitivity;
	uint32_t				uiDoubleClickTime;
	bool					bDoubleClicked[8];
	uint32_t				uiLastClickedTime[8];
	ControllerVibration*	pControllerVibration;
	VibrationStates*		pVibrationProperties;
	bool					bIsControllerEnabled;
	uint32_t				eActionToSet;
	bool					bSettingAControl;
	bool					bUpdated;
	Bitfield16				usOverrideFlags;
	uint8_t					ucKeyBinds[Devices::COUNT][28];


	static Controls* GetSingleton();

	static LPDIRECTINPUT8A GetDirectInput();
	static LPDIRECTINPUTDEVICE8A GetKeyboard();
	static LPDIRECTINPUTDEVICE8A GetMouse();

	void ToggleMouse(bool abDisable);
	void ToggleKeyboard(bool abDisable);
	void ChangeMouseCooperativeLevel(bool abBackground);
	bool GetButtonState(uint32_t auiInputDevice, uint8_t aucKey, uint32_t auiState) const;
	int32_t GetControlState(uint8_t aucKey, uint32_t auiState) const;
	int32_t GetControllerValue(uint32_t auiInputDevice, uint32_t aucInput) const;
	int32_t GetMouseAxis(uint32_t aeMouseAxis) const;
	int32_t GetMouseButton(int32_t aiButton, int32_t aiToggleFlags) const;
};

ASSERT_SIZE(Controls, 0x1C04);