#pragma once

#include <string>
#include <vector>

enum InputEvent
{
	Pressed = 0,
	Released = 1,
	Repeat = 2,
	DoubleClick = 3,
	Axis = 4
};


const std::vector<std::string> possibleKeyNames = {
	"",
	"XboxTypeS_DPad_Up",
	"XboxTypeS_DPad_Left",
	"XboxTypeS_DPad_Right",
	"XboxTypeS_DPad_Down",
	"XboxTypeS_A",
	"XboxTypeS_B",
	"XboxTypeS_X",
	"XboxTypeS_Y",
	"XboxTypeS_LeftShoulder",
	"XboxTypeS_RightShoulder",
	"XboxTypeS_Back",
	"XboxTypeS_Start",
	"XboxTypeS_LeftX",
	"XboxTypeS_LeftY",
	"XboxTypeS_RightX",
	"XboxTypeS_RightY"
	"XboxTypeS_LeftTrigger",
	"XboxTypeS_RightTrigger",
	"XboxTypeS_LeftTriggerAxis",
	"XboxTypeS_RightTriggerAxis",
	"XboxTypeS_LeftThumbStick",
	"XboxTypeS_RightThumbStick",
	"F1",
	"F2",
	"F3",
	"F4",
	"F5",
	"F6",
	"F7",
	"F8",
	"F9",
	"F10",
	"F11",
	"F12",
	"A",
	"B",
	"C",
	"D",
	"E",
	"F",
	"G",
	"H",
	"I",
	"J",
	"K",
	"L",
	"M",
	"N",
	"O",
	"P",
	"Q",
	"R",
	"S",
	"T",
	"U",
	"V",
	"W",
	"X",
	"Y",
	"Z",
	"Escape",
	"Tab",
	"Tilde",
	"ScrollLock",
	"Pause",
	"One",
	"Two",
	"Three",
	"Four",
	"Five",
	"Six",
	"Seven",
	"Eight",
	"Nine",
	"Zero",
	"Underscore",
	"Equals",
	"Backslash",
	"LeftBracket",
	"RightBracket",
	"Enter",
	"CapsLock",
	"Semicolon",
	"Quote",
	"LeftShift",
	"Comma",
	"Period",
	"Slash",
	"RightShift",
	"LeftControl",
	"LeftAlt",
	"SpaceBar",
	"RightAlt",
	"RightControl",
	"Left",
	"Up",
	"Down",
	"Right",
	"Home",
	"End",
	"Insert",
	"PageUp",
	"Delete",
	"PageDown",
	"NumLock",
	"Divide",
	"Multiply",
	"Subtract",
	"Add",
	"PageDown",
	"NumPadOne",
	"NumPadTwo",
	"NumPadThree",
	"NumPadFour",
	"NumPadFive",
	"NumPadSix",
	"NumPadSeven",
	"NumPadEight",
	"NumPadNine",
	"NumPadZero",
	"Decimal",
	"LeftMouseButton",
	"RightMouseButton",
	"ThumbMouseButton",
	"ThumbMouseButton2",
	"MouseScrollUp",
	"MouseScrollDown",
	"MouseX",
	"MouseY"
};