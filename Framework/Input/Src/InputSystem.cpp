#include "Precompiled.h"
#include "InputSystem.h"

using namespace NFGE;
using namespace NFGE::Input;

namespace
{
	std::unique_ptr<InputSystem> sInputSystem = nullptr;
	Core::WindowMessageHandler sWindowMessageHandler;
}

BOOL CALLBACK Input::EnumGamePadCallback(const DIDEVICEINSTANCE* pDIDeviceInstance, VOID* pContext)
{
	// Obtain an interface to the enumerated joystick
	InputSystem* inputSystem = static_cast<InputSystem*>(pContext);
	IDirectInput8* pDI = inputSystem->mDirectInput;
	IDirectInputDevice8** pGamePad = &(inputSystem->mGamePadDevice);
	if (FAILED(pDI->CreateDevice(pDIDeviceInstance->guidInstance, pGamePad, nullptr))) 
	{
		LOG("[InputSystem] Failed to create game pad device.");
	}

	return DIENUM_STOP;
}

LRESULT CALLBACK Input::InputSystemMessageHandler(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (sInputSystem)
	{
		switch (message)
		{
			case WM_ACTIVATEAPP:
			{
				if (wParam == TRUE)
				{
					SetCapture(window);
				}
				else
				{
					sInputSystem->mMouseLeftEdge = false;
					sInputSystem->mMouseRightEdge = false;
					sInputSystem->mMouseTopEdge = false;
					sInputSystem->mMouseBottomEdge = false;
					ReleaseCapture();
				}
				break;
			}
			case WM_LBUTTONDOWN:
			{
				sInputSystem->mCurrMouseButtons[0] = true;
				break;
			}
			case WM_LBUTTONUP:
			{
				sInputSystem->mCurrMouseButtons[0] = false;
				break;
			}
			case WM_RBUTTONDOWN:
			{
				sInputSystem->mCurrMouseButtons[1] = true;
				break;
			}
			case WM_RBUTTONUP:
			{
				sInputSystem->mCurrMouseButtons[1] = false;
				break;
			}
			case WM_MBUTTONDOWN:
			{
				sInputSystem->mCurrMouseButtons[2] = true;
				break;
			}
			case WM_MBUTTONUP:
			{
				sInputSystem->mCurrMouseButtons[2] = false;
				break;
			}
			case WM_MOUSEWHEEL:
			{
				sInputSystem->mMouseWheel += GET_WHEEL_DELTA_WPARAM(wParam);
				break;
			}
			case WM_MOUSEMOVE:
			{
				int mouseX = (signed short)(lParam);
				int mouseY = (signed short)(lParam >> 16);

				sInputSystem->mCurrMouseX = mouseX;
				sInputSystem->mCurrMouseY = mouseY;
				if (sInputSystem->mPrevMouseX == -1)
				{
					sInputSystem->mPrevMouseX = mouseX;
					sInputSystem->mPrevMouseY = mouseY;
				}

				RECT rect;
				GetClientRect(window, &rect);
				sInputSystem->mMouseLeftEdge = mouseX <= rect.left;
				sInputSystem->mMouseRightEdge = mouseX + 1 >= rect.right;
				sInputSystem->mMouseTopEdge = mouseY <= rect.top;
				sInputSystem->mMouseBottomEdge = mouseY + 1 >= rect.bottom;
				break;
			}
			case WM_KEYDOWN:
			{
				if (wParam < 256)
				{
					sInputSystem->mCurrKeys[wParam] = true;
				}
				break;
			}
			case WM_KEYUP:
			{
				if (wParam < 256)
				{
					sInputSystem->mCurrKeys[wParam] = false;
				}
				break;
			}
		}
	}

	return sWindowMessageHandler.ForwardMessage(window, message, wParam, lParam);
}

void InputSystem::StaticInitialize(HWND window)
{
	ASSERT(sInputSystem == nullptr, "[InputSystem] System already initialized!");
	sInputSystem = std::make_unique<InputSystem>();
	sInputSystem->Initialize(window);
}

void InputSystem::StaticTerminate()
{
	if (sInputSystem != nullptr)
	{
		sInputSystem->Terminate();
		sInputSystem.reset();
	}
}

InputSystem* InputSystem::Get()
{
	ASSERT(sInputSystem != nullptr, "[InputSystem] No system registered.");
	return sInputSystem.get();
}

InputSystem::InputSystem()
	: mDirectInput(nullptr)
	, mGamePadDevice(nullptr)
	, mCurrMouseX(-1)
	, mCurrMouseY(-1)
	, mPrevMouseX(-1)
	, mPrevMouseY(-1)
	, mMouseMoveX(0)
	, mMouseMoveY(0)
	, mMouseLeftEdge(false)
	, mMouseRightEdge(false)
	, mMouseTopEdge(false)
	, mMouseBottomEdge(false)
	, mInitialized(false)
{
	ZeroMemory(&mCurrKeys, sizeof(mCurrKeys));
	ZeroMemory(&mPrevKeys, sizeof(mPrevKeys));
	ZeroMemory(&mPressedKeys, sizeof(mPressedKeys));
	ZeroMemory(&mCurrMouseButtons, sizeof(mCurrMouseButtons));
	ZeroMemory(&mPrevMouseButtons, sizeof(mPrevMouseButtons));
	ZeroMemory(&mPressedMouseButtons, sizeof(mPressedMouseButtons));
	ZeroMemory(&mCurrGamePadState, sizeof(DIJOYSTATE));
	ZeroMemory(&mPrevGamePadState, sizeof(DIJOYSTATE));
}

InputSystem::~InputSystem()
{
	ASSERT(!mInitialized, "[InputSystem] Terminate() must be called to clean up!");
}

void InputSystem::Initialize(HWND window)
{
	// Check if we have already initialized the system
	if (mInitialized)
	{
		LOG("[InputSystem] System already initialized.");
		return;
	}

	LOG("[InputSystem] Initializing...");
	
	// Hook application to window's procedure
	sWindowMessageHandler.Hook(window, InputSystemMessageHandler);

	// Obtain an interface to DirectInput
	HRESULT hr = DirectInput8Create(GetModuleHandle(nullptr), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&mDirectInput, nullptr);
	ASSERT(SUCCEEDED(hr), "[InputSystem] Failed to create DirectInput object.");

	//----------------------------------------------------------------------------------------------------
	// Enumerate for game pad device
	if (FAILED(mDirectInput->EnumDevices(DI8DEVCLASS_GAMECTRL, EnumGamePadCallback, this, DIEDFL_ATTACHEDONLY)))
	{
		LOG("[InputSystem] Failed to enumerate for game pad devices.");
	}

	// Check if we have a game pad detected
	if (mGamePadDevice != nullptr)
	{
		// Set the game pad data format
		hr = mGamePadDevice->SetDataFormat(&c_dfDIJoystick);
		ASSERT(SUCCEEDED(hr), "[InputSystem] Failed to set game pad data format.");

		// Set the game pad cooperative level
		hr = mGamePadDevice->SetCooperativeLevel(window, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
		ASSERT(SUCCEEDED(hr), "[InputSystem] Failed to set game pad cooperative level.");

		// Acquire the game pad device
		hr = mGamePadDevice->Acquire();
		ASSERT(SUCCEEDED(hr), "[InputSystem] Failed to acquire game pad device.");
	}
	else
	{
		LOG("[InputSystem] No game pad attached.");
	}

	// Set flag
	mInitialized = true;

	LOG("[InputSystem] System initialized.");
}

void InputSystem::Terminate()
{
	// Check if we have already terminated the system
	if (!mInitialized)
	{
		LOG("[InputSystem] System already terminated.");
		return;
	}

	LOG("[InputSystem] Terminating...");

	// Release devices
	if (mGamePadDevice != nullptr)
	{
		mGamePadDevice->Unacquire();
		mGamePadDevice->Release();
		mGamePadDevice = nullptr;
	}

	SafeRelease(mDirectInput);

	// Restore original window's procedure
	sWindowMessageHandler.Unhook();

	// Set flag
	mInitialized = false;

	LOG("[InputSystem] System terminated.");
}

void InputSystem::Update()
{
	ASSERT(mInitialized, "[InputSystem] System not initialized.");

	// Store the previous keyboard state
	for (int i = 0; i < 512; ++i)
	{
		mPressedKeys[i] = !mPrevKeys[i] && mCurrKeys[i];
	}
	memcpy(mPrevKeys, mCurrKeys, sizeof(mCurrKeys));

	// Update mouse movement
	mMouseMoveX = mCurrMouseX - mPrevMouseX;
	mMouseMoveY = mCurrMouseY - mPrevMouseY;
	mPrevMouseX = mCurrMouseX;
	mPrevMouseY = mCurrMouseY;

	// Store the previous mouse state
	for (int i = 0; i < 3; ++i)
	{
		mPressedMouseButtons[i] = !mPrevMouseButtons[i] && mCurrMouseButtons[i];
	}
	memcpy(mPrevMouseButtons, mCurrMouseButtons, sizeof(mCurrMouseButtons));

	// Update game pad
	if (mGamePadDevice != nullptr)
	{
		UpdateGamePad();
	}
}

bool InputSystem::IsKeyDown(KeyCode key) const
{
	return mCurrKeys[(int)key];
}

bool InputSystem::IsKeyPressed(KeyCode key) const
{
	return mPressedKeys[(int)key];
}

bool InputSystem::IsMouseDown(MouseButton button) const
{
	return mCurrMouseButtons[(int)button];
}

bool InputSystem::IsMousePressed(MouseButton button) const
{
	return mPressedMouseButtons[(int)button];
}

int InputSystem::GetMouseScreenX() const
{
	return mCurrMouseX;
}

int InputSystem::GetMouseScreenY() const
{
	return mCurrMouseY;
}

bool InputSystem::IsMouseLeftEdge() const
{
	return mMouseLeftEdge;
}

bool InputSystem::IsMouseRightEdge() const
{
	return mMouseRightEdge;
}

bool InputSystem::IsMouseTopEdge() const
{
	return mMouseTopEdge;
}

bool InputSystem::IsMouseBottomEdge() const
{
	return mMouseBottomEdge;
}

int InputSystem::GetMouseMoveX() const
{
	return mMouseMoveX;
}

int InputSystem::GetMouseMoveY() const
{
	return mMouseMoveY;
}

int InputSystem::GetMouseMoveZ() const
{
	return mMouseWheel;
}

bool InputSystem::IsGamePadButtonDown(GamePadButton button) const
{
	return (mCurrGamePadState.rgbButtons[(uint32_t)button] & 0x80) != 0;
}

bool InputSystem::IsGamePadButtonPressed(GamePadButton button) const
{
	const bool currState = (mCurrGamePadState.rgbButtons[(uint32_t)button] & 0x80) != 0;
	const bool prevState = (mPrevGamePadState.rgbButtons[(uint32_t)button] & 0x80) != 0;
	return !prevState && currState;
}

bool InputSystem::IsDPadUp() const
{
	const bool hasGamePad = (mGamePadDevice != nullptr);
	return hasGamePad && (mCurrGamePadState.rgdwPOV[0] == 0);
}

bool InputSystem::IsDPadDown() const
{
	return (mCurrGamePadState.rgdwPOV[0] == 18000);
}

bool InputSystem::IsDPadLeft() const
{
	return (mCurrGamePadState.rgdwPOV[0] == 27000);
}

bool InputSystem::IsDPadRight() const
{
	return (mCurrGamePadState.rgdwPOV[0] == 9000);
}

float InputSystem::GetLeftAnalogX() const
{
	return (mCurrGamePadState.lX / 32767.5f) - 1.0f;
}

float InputSystem::GetLeftAnalogY() const
{
	return -(mCurrGamePadState.lY / 32767.5f) + 1.0f;
}

float InputSystem::GetRightAnalogX() const
{
	return (mCurrGamePadState.lZ / 32767.5f) - 1.0f;
}

float InputSystem::GetRightAnalogY() const
{
	return -(mCurrGamePadState.lRz / 32767.5f) + 1.0f;
}

bool NFGE::Input::InputSystem::IsAnyKeyPressed() const
{
	bool ret = false;
	for (size_t i = 0; i < 512; i++)
	{
		if (mPressedKeys[i])
			return true;
	}
	return false;
}

void InputSystem::UpdateGamePad()
{
	// Store the previous game pad state
	memcpy(&mPrevGamePadState, &mCurrGamePadState, sizeof(DIJOYSTATE));

	// Poll the game pad device
	static bool sWriteToLog = true;
	HRESULT hr = mGamePadDevice->Poll();
	if (FAILED(hr))
	{   
		// Check if the device is lost
		if (DIERR_INPUTLOST == hr || DIERR_NOTACQUIRED == hr)
		{
			if (sWriteToLog)
			{
				LOG("[InputSystem] Game pad device is lost.");
				sWriteToLog = false;
			}

			// Try to acquire game pad device again
			mGamePadDevice->Acquire();
		}
		else
		{
			LOG("[InputSystem] Failed to get game pad state.");
			return;
		}
	}
	else
	{
		// Reset flag
		sWriteToLog = true;
	}

	// Get game pad state
	hr = mGamePadDevice->GetDeviceState(sizeof(DIJOYSTATE), (void*)&mCurrGamePadState);
	if (FAILED(hr))
	{   
		// Check if the device is lost
		if (DIERR_INPUTLOST == hr || DIERR_NOTACQUIRED == hr)
		{
			if (sWriteToLog)
			{
				LOG("[InputSystem] Game pad device is lost.");
				sWriteToLog = false;
			}

			// Try to acquire game pad device again
			mGamePadDevice->Acquire();
		}
		else
		{
			LOG("[InputSystem] Failed to get game pad state.");
			return;
		}
	}
	else
	{
		// Reset flag
		sWriteToLog = true;
	}
}