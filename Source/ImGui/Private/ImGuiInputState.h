// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "ImGuiInteroperability.h"
#include "Utilities/Arrays.h"


// Collects and stores input state and updates for ImGui IO.
class FImGuiInputState
{
public:

	// Characters buffer.
	using FCharactersBuffer = ImGuiInterops::ImGuiTypes::FInputCharactersBuffer;

	// Array for mouse button states.
	using FMouseButtonsArray = ImGuiInterops::ImGuiTypes::FMouseButtonsArray;

	// Array for key states.
	using FKeysArray = ImGuiInterops::ImGuiTypes::FKeysArray;

	// Array for navigation input states.
	using FNavInputArray = ImGuiInterops::ImGuiTypes::FNavInputArray;

	// Pair of indices defining range in mouse buttons array.
	using FMouseButtonsIndexRange = Utilities::TArrayIndexRange<FMouseButtonsArray, uint32>;

	// Pair of indices defining range in keys array.
	using FKeysIndexRange = Utilities::TArrayIndexRange<FKeysArray, uint32>;

	// Create empty state with whole range instance with the whole update state marked as dirty.
	FImGuiInputState();

	// Get reference to input characters buffer.
	const FCharactersBuffer& GetCharacters() const { return InputCharacters; }

	// Get number of characters in input characters buffer.
	int32 GetCharactersNum()  const { return InputCharactersNum; }

	// Add a character to the characters buffer. We can store and send to ImGui up to 16 characters per frame. Any
	// character beyond that limit will be discarded.
	// @param Char - Character to add
	void AddCharacter(TCHAR Char);

	// Get reference to the array with key down states.
	const FKeysArray& GetKeys() const { return KeysDown; }

	// Get possibly empty range of indices bounding dirty part of the keys array.
	const FKeysIndexRange& GetKeysUpdateRange() const { return KeysUpdateRange; }

	// Change state of the key in the keys array and expand range bounding dirty part of the array.
	// @param KeyEvent - Key event representing the key
	// @param bIsDown - True, if key is down
	void SetKeyDown(const FKeyEvent& KeyEvent, bool bIsDown) { SetKeyDown(ImGuiInterops::GetKeyIndex(KeyEvent), bIsDown); }

	// Change state of the key in the keys array and expand range bounding dirty part of the array.
	// @param Key - Keyboard key
	// @param bIsDown - True, if key is down
	void SetKeyDown(const FKey& Key, bool bIsDown) { SetKeyDown(ImGuiInterops::GetKeyIndex(Key), bIsDown); }

	// Get reference to the array with mouse button down states.
	const FMouseButtonsArray& GetMouseButtons() const { return MouseButtonsDown; }

	// Get possibly empty range of indices bounding dirty part of the mouse buttons array.
	const FMouseButtonsIndexRange& GetMouseButtonsUpdateRange() const { return MouseButtonsUpdateRange; }

	// Change state of the button in the mouse buttons array and expand range bounding dirty part of the array.
	// @param MouseEvent - Mouse event representing mouse button
	// @param bIsDown - True, if button is down
	void SetMouseDown(const FPointerEvent& MouseEvent, bool bIsDown) { SetMouseDown(ImGuiInterops::GetMouseIndex(MouseEvent), bIsDown); }

	// Change state of the button in the mouse buttons array and expand range bounding dirty part of the array.
	// @param MouseButton - Mouse button key
	// @param bIsDown - True, if button is down
	void SetMouseDown(const FKey& MouseButton, bool bIsDown) { SetMouseDown(ImGuiInterops::GetMouseIndex(MouseButton), bIsDown); }

	// Get mouse wheel delta accumulated during the last frame.
	float GetMouseWheelDelta() const { return MouseWheelDelta; }

	// Add mouse wheel delta.
	// @param DeltaValue - Mouse wheel delta to add
	void AddMouseWheelDelta(float DeltaValue) { MouseWheelDelta += DeltaValue; }

	// Get the current mouse position.
	const FVector2D& GetMousePosition() const { return MousePosition; }

	// Set mouse position.
	// @param Position - New mouse position
	void SetMousePosition(const FVector2D& Position) { MousePosition = Position; }

	// Check whether input has active mouse pointer.
	bool HasMousePointer() const { return bHasMousePointer; }

	// Set whether input has active mouse pointer.
	// @param bHasPointer - True, if input has active mouse pointer
	void SetMousePointer(bool bInHasMousePointer) { bHasMousePointer = bInHasMousePointer; }

	// Get Control down state.
	bool IsControlDown() const { return bIsControlDown; }

	// Set Control down state.
	// @param bIsDown - True, if Control is down
	void SetControlDown(bool bIsDown) { bIsControlDown = bIsDown; }

	// Get Shift down state.
	bool IsShiftDown() const { return bIsShiftDown; }

	// Set Shift down state.
	// @param bIsDown - True, if Shift is down
	void SetShiftDown(bool bIsDown) { bIsShiftDown = bIsDown; }

	// Get Alt down state.
	bool IsAltDown() const { return bIsAltDown; }

	// Set Alt down state.
	// @param bIsDown - True, if Alt is down
	void SetAltDown(bool bIsDown) { bIsAltDown = bIsDown; }

	// Get reference to the array with navigation input states.
	const FNavInputArray& GetNavigationInputs() const { return NavigationInputs; }

	// Change state of the navigation input associated with this gamepad key.
	// @param KeyEvent - Key event with gamepad key input
	// @param bIsDown - True, if key is down
	void SetGamepadNavigationKey(const FKeyEvent& KeyEvent, bool bIsDown) { ImGuiInterops::SetGamepadNavigationKey(NavigationInputs, KeyEvent.GetKey(), bIsDown); }

	// Change state of the navigation input associated with this gamepad axis.
	// @param AnalogInputEvent - Analogue input event with gamepad axis input
	// @param Value - Analogue value that should be set for this axis
	void SetGamepadNavigationAxis(const FAnalogInputEvent& AnalogInputEvent, float Value) { ImGuiInterops::SetGamepadNavigationAxis(NavigationInputs, AnalogInputEvent.GetKey(), Value); }

	// Check whether keyboard navigation is enabled.
	bool IsKeyboardNavigationEnabled() const { return bKeyboardNavigationEnabled; }

	// Set whether keyboard navigation is enabled.
	// @param bEnabled - True, if navigation is enabled
	void SetKeyboardNavigationEnabled(bool bEnabled) { bKeyboardNavigationEnabled = bEnabled; }

	// Check whether gamepad navigation is enabled.
	bool IsGamepadNavigationEnabled() const { return bGamepadNavigationEnabled; }

	// Set whether gamepad navigation is enabled.
	// @param bEnabled - True, if navigation is enabled
	void SetGamepadNavigationEnabled(bool bEnabled) { bGamepadNavigationEnabled = bEnabled; }

	// Check whether gamepad is attached.
	bool HasGamepad() const { return bHasGamepad; }

	// Set whether gamepad is attached.
	// @param bInHasGamepad - True, if gamepad is attached
	void SetGamepad(bool bInHasGamepad) { bHasGamepad = bInHasGamepad; }

	// Reset the whole state and mark as dirty.
	void ResetState() { Reset(true, true, true); }

	// Reset keyboard state and mark as dirty.
	void ResetKeyboardState() { Reset(true, false, false); }

	// Reset mouse state and mark as dirty.
	void ResetMouseState() { Reset(false, true, false); }

	// Reset navigation state.
	void ResetNavigationState() { Reset(false, false, true); }

	// Clear part of the state that is meant to be updated in every frame like: accumulators, buffers, navigation data
	// and information about dirty parts of keys or mouse buttons arrays.
	void ClearUpdateState();

private:

	void SetKeyDown(uint32 KeyIndex, bool bIsDown);
	void SetMouseDown(uint32 MouseIndex, bool IsDown);

	void Reset(bool bKeyboard, bool bMouse, bool bNavigation);

	void ClearCharacters();
	void ClearKeys();
	void ClearMouseButtons();
	void ClearMouseAnalogue();
	void ClearModifierKeys();
	void ClearNavigationInputs();

	FVector2D MousePosition = FVector2D::ZeroVector;
	float MouseWheelDelta = 0.f;

	FMouseButtonsArray MouseButtonsDown;
	FMouseButtonsIndexRange MouseButtonsUpdateRange;

	FCharactersBuffer InputCharacters;
	uint32 InputCharactersNum = 0;

	FKeysArray KeysDown;
	FKeysIndexRange KeysUpdateRange;

	FNavInputArray NavigationInputs;

	bool bHasMousePointer = false;

	bool bIsControlDown = false;
	bool bIsShiftDown = false;
	bool bIsAltDown = false;

	bool bKeyboardNavigationEnabled = false;
	bool bGamepadNavigationEnabled = false;
	bool bHasGamepad = false;
};
