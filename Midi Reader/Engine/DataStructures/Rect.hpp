#pragma once
#include <Windows.h>
#include <string>
#include <optional>
#include "Vec.hpp"
#include "..\Log.hpp"
#include "..\Midi\Midi.hpp"

class Vec2;

enum CHARACTER_COLOR : unsigned short
{
	//Foreground colors
	FG_COLOR_BLACK = 0x00,
	FG_COLOR_BLUE = 0x01,
	FG_COLOR_GREEN = 0x02,
	FG_COLOR_AQUA = 0x03,
	FG_COLOR_RED = 0x04,
	FG_COLOR_PURPLE = 0x05,
	FG_COLOR_YELLOW = 0x06,
	FG_COLOR_LIGHT_GRAY = 0x07,
	FG_COLOR_GRAY = 0x08,
	FG_COLOR_LIGHT_BLUE = 0x09,
	FG_COLOR_LIGHT_GREEN = 0x0A,
	FG_COLOR_LIGHT_AQUA = 0x0B,
	FG_COLOR_LIGHT_RED = 0x0C,
	FG_COLOR_LIGHT_PURPLE = 0x0D,
	FG_COLOR_LIGHT_YELLOW = 0x0E,
	FG_COLOR_WHITE = 0x0F,
	//Background colors
	BG_COLOR_BLACK = 0x00,
	BG_COLOR_BLUE = 0x10,
	BG_COLOR_GREEN = 0x20,
	BG_COLOR_AQUA = 0x30,
	BG_COLOR_RED = 0x40,
	BG_COLOR_PURPLE = 0x50,
	BG_COLOR_YELLOW = 0x60,
	BG_COLOR_LIGHT_GRAY = 0x70,
	BG_COLOR_GRAY = 0x80,
	BG_COLOR_LIGHT_BLUE = 0x90,
	BG_COLOR_LIGHT_GREEN = 0xA0,
	BG_COLOR_LIGHT_AQUA = 0xB0,
	BG_COLOR_LIGHT_RED = 0xC0,
	BG_COLOR_LIGHT_PURPLE = 0xD0,
	BG_COLOR_LIGHT_YELLOW = 0xE0,
	BG_COLOR_WHITE = 0xF0
};

enum CHARACTER_TYPE
{
	CHARACTER_FULL = 0x2588,
	CHARACTER_THREE_QUARTERS = 0x2593,
	CHARACTER_HALF = 0x2592,
	CHARACTER_QUARTER = 0x2591
};

enum PIPE_CHARACTER
{
	DOUBLE_PIPE_HORIZONTAL = 0x2550,
	DOUBLE_PIPE_VERTICAL = 0x2551,
	DOUBLE_PIPE_DOWN_RIGHT = 0x2554,
	DOUBLE_PIPE_DOWN_LEFT = 0x2557,
	DOUBLE_PIPE_UP_RIGHT = 0x255A,
	DOUBLE_PIPE_UP_LEFT = 0x255D,

	SINGLE_PIPE_HORIZONTAL = 0x2500,
	SINGLE_PIPE_VERTICAL = 0x2502,
	SINGLE_PIPE_DOWN_RIGHT = 0x250C,
	SINGLE_PIPE_DOWN_LEFT = 0x2510,
	SINGLE_PIPE_UP_RIGHT = 0x2514,
	SINGLE_PIPE_UP_LEFT = 0x2518,

	WIDE_SINGLE_PIPE_HORIZONTAL = 0x2501,
	WIDE_SINGLE_PIPE_VERTICAL = 0x2503,
	WIDE_SINGLE_PIPE_DOWN_RIGHT = 0x250F,
	WIDE_SINGLE_PIPE_DOWN_LEFT = 0x2513,
	WIDE_SINGLE_PIPE_UP_RIGHT = 0x2517,
	WIDE_SINGLE_PIPE_UP_LEFT = 0x251B,
};

class Rect
{
private:
	Vec2* position;
	Vec2* dimension;
	CHAR_INFO* buffer;
	unsigned int bufferSize;

public:
	Rect(int x, int y, int width, int height);
	Rect(const Rect& other);
	Rect& operator=(const Rect& other);
	~Rect();
	Vec2 GetPosition() const;
	Vec2 GetDimension() const;
	CHAR_INFO* GetBuffer() const;
	std::optional<CHAR_INFO> GetPixel(int x, int y) const;
	std::optional<CHAR_INFO> GetPixel(Vec2 pos) const;
	void SetPosition(int x, int y);
	void SetPosition(Vec2 pos);
	void SetDimension(unsigned int x, unsigned int y);
	void Move(int x, int y);
	void Move(Vec2 pos);

	//Draw Methods
	void ClearBuffer();
	void Draw(int x, int y);
	void Draw(int x, int y, unsigned short character);
	void Draw(int x, int y, unsigned short character, unsigned short color);
	void FillArea(int x, int y, int width, int height, unsigned short character, unsigned short color);
	void FillArea(int x, int y, int width, int height, unsigned short character);
	void Fill(unsigned short character, unsigned short color);
	void FillWithText(const std::string& string, unsigned short color);
	void FillWithText(const std::string& string);
	void DrawRect(int x, int y, CHAR_INFO* data, int width, int height);
	void DrawRect(const Rect& rect);
	void DrawRectTransparent(int x, int y, CHAR_INFO* buffer, int width, int height, unsigned short exeptionColor);
	void DrawRectTransparent(const Rect& rect, unsigned short exeptionColor);
	void DrawString(int x, int y, const std::string& string);
	void DrawString(int x, int y, const std::string& string, unsigned short color);
	void DrawLine(int x0, int y0, int x1, int y1);
	void DrawLine(int x0, int y0, int x1, int y1, unsigned short character, unsigned short color);

	//Draw Piano Keys
	void DrawPiano(int x, int y, int octaves, const PianoKeyColor& keyColor);
	void DrawPianoKeyWhite1(int x, int y, unsigned short character, unsigned short color);
	void DrawPianoKeyWhite2(int x, int y, unsigned short character, unsigned short color);
	void DrawPianoKeyBlack(int x, int y, unsigned short character, unsigned short color);

private:
	void UnsecureDraw(int x, int y, unsigned short character, unsigned short color);
};
