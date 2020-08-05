#ifndef INCLUDED_NFGE_INPUT_INPUTSYSTEM_H
#define INCLUDED_NFGE_INPUT_INPUTSYSTEM_H

#include "InputTypes.h"

namespace NFGE::Input {

class InputSystem
{
public:
	static void StaticInitialize(HWND window);
	static void StaticTerminate();
	static InputSystem* Get();

public:
	InputSystem();
	~InputSystem();

	InputSystem(const InputSystem&) = delete;
	InputSystem& operator=(const InputSystem&) = delete;

	void Initialize(HWND window);
	void Terminate();

	void Update();

	bool IsKeyDown(KeyCode key) const;
	bool IsKeyPressed(KeyCode key) const;

	bool IsMouseDown(MouseButton button) const;
	bool IsMousePressed(MouseButton button) const;

	int GetMouseScreenX() const;
	int GetMouseScreenY() const;

	bool IsMouseLeftEdge() const;
	bool IsMouseRightEdge() const;
	bool IsMouseTopEdge() const;
	bool IsMouseBottomEdge() const;

	int GetMouseMoveX() const;
	int GetMouseMoveY() const;
	int GetMouseMoveZ() const;

	bool IsGamePadButtonDown(GamePadButton button) const;
	bool IsGamePadButtonPressed(GamePadButton button) const;

	bool IsDPadUp() const;
	bool IsDPadDown() const;
	bool IsDPadLeft() const;
	bool IsDPadRight() const;

	float GetLeftAnalogX() const;
	float GetLeftAnalogY() const;
	float GetRightAnalogX() const;
	float GetRightAnalogY() const;

	bool IsAnyKeyPressed() const;

private:
	friend BOOL CALLBACK EnumGamePadCallback(const DIDEVICEINSTANCE* pDIDeviceInstance, VOID* pContext);
	friend LRESULT CALLBACK InputSystemMessageHandler(HWND window, UINT message, WPARAM wParam, LPARAM lParam);

	void UpdateGamePad();

	IDirectInput8* mDirectInput;
	IDirectInputDevice8* mGamePadDevice;

	DIJOYSTATE mCurrGamePadState;
	DIJOYSTATE mPrevGamePadState;

	bool mCurrKeys[512];
	bool mPrevKeys[512];
	bool mPressedKeys[512];

	int mCurrMouseX;
	int mCurrMouseY;
	int mPrevMouseX;
	int mPrevMouseY;
	int mMouseMoveX;
	int mMouseMoveY;

	int mMouseWheel;

	bool mCurrMouseButtons[3];
	bool mPrevMouseButtons[3];
	bool mPressedMouseButtons[3];

	bool mMouseLeftEdge;
	bool mMouseRightEdge;
	bool mMouseTopEdge;
	bool mMouseBottomEdge;
	bool mIsCenterMouse;

	bool mInitialized;
};

} // namespace Siege::Input

#endif // #ifndef INCLUDED_SIEGE_INPUT_INPUTSYSTEM_H