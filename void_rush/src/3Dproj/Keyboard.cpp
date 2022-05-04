#include "Keyboard.h"
#include <iostream>

bool getkey(char Key)
{
	return (GetKeyState(Key) & 0x8000);
}

Keyboard::Keyboard()
{
	for (int i = 0; i < 256; i++) {
		KeysDown[i] = false;
		oKeyReleased[i] = false;
	}
	type = EventType::Invalid;
}

bool Keyboard::isKeyPressed(unsigned char letter)
{
	return KeysDown[letter];
}

bool Keyboard::onceisKeyReleased(unsigned char letter)
{
	bool ret = oKeyReleased[letter];
	oKeyReleased[letter] = false;
	return ret;
}

void Keyboard::onKeyReleased(const unsigned char letter)
{
	KeysDown[letter] = false;
	oKeyReleased[letter] = true;
}

void Keyboard::onKeyDown(const unsigned char letter)
{
	KeysDown[letter] = true;
	oKeyReleased[letter] = false;
}
