#include "Rect.hpp"

Rect::Rect(int x, int y, int width, int height)
{
	width = width <= 0 ? 1 : width;
	height = height <= 0 ? 1 : height;
	position = new Vec2(x, y);
	dimension = new Vec2(width, height);
	buffer = new CHAR_INFO[width * height];
	bufferSize = width * height;
	memset(buffer, 0, sizeof(CHAR_INFO) * width * height);
}

Rect::Rect(const Rect& other)
{
	position = new Vec2(other.position->x, other.position->y);
	dimension = new Vec2(other.dimension->x, other.dimension->y);
	buffer = new CHAR_INFO[other.dimension->x * other.dimension->y];
	bufferSize = other.dimension->x * other.dimension->y;
	memcpy(buffer, other.buffer, sizeof(CHAR_INFO) * other.dimension->x * other.dimension->y);
}

Rect& Rect::operator=(const Rect& other)
{
	if (this == &other)
		return *this;
	this->SetPosition(other.GetPosition().x, other.GetPosition().y);
	this->SetDimension(other.GetDimension().x, other.GetDimension().y);
	memcpy(this->GetBuffer(), other.GetBuffer(), sizeof(CHAR_INFO) * this->GetDimension().x * this->GetDimension().y);
	return *this;
}

Rect::~Rect()
{
	delete position;
	delete dimension;
	delete[] buffer;
}

Vec2 Rect::GetPosition() const
{
	return *position;
}

Vec2 Rect::GetDimension() const
{
	return *dimension;
}

CHAR_INFO* Rect::GetBuffer() const
{
	return buffer;
}

std::optional<CHAR_INFO> Rect::GetPixel(int x, int y) const
{
	if (x < dimension->x && x >= 0 && y < dimension->y && y >= 0)
	{
		return buffer[y * short(dimension->x) + x];
	}
	return {};
}

std::optional<CHAR_INFO> Rect::GetPixel(Vec2 pos) const
{
	return GetPixel(pos.x, pos.y);
}

void Rect::SetPosition(int x, int y)
{
	position->x = x;
	position->y = y;
}

void Rect::SetPosition(Vec2 pos)
{
	*position = pos;
}

void Rect::SetDimension(unsigned int x, unsigned int y)
{
	if ((x * y) > bufferSize) {
		Log log("RectLog.txt");
		log.Warn("In file Rect.cpp in line: " + std::to_string(__LINE__) + " Failed to set rect dimension");
		return;
	}
	dimension->x = x;
	dimension->y = y;
}

void Rect::Move(int x, int y)
{
	position->x += x;
	position->y += y;
}

void Rect::Move(Vec2 pos)
{
	Move(pos.x, pos.y);
}

void Rect::ClearBuffer()
{
	memset(buffer, 0, sizeof(CHAR_INFO) * dimension->x * dimension->y);
}

void Rect::Draw(int x, int y)
{
	Draw(x, y, CHARACTER_FULL, FG_COLOR_WHITE);
}

void Rect::Draw(int x, int y, unsigned short character)
{
	Draw(x, y, character, FG_COLOR_WHITE);
}

void Rect::Draw(int x, int y, unsigned short character, unsigned short color)
{
	if (x < dimension->x && x >= 0 && y < dimension->y && y >= 0)
	{
		buffer[y * short(dimension->x) + x].Char.UnicodeChar = character;
		buffer[y * short(dimension->x) + x].Attributes = color;
	}
}

void Rect::FillArea(int x, int y, int width, int height, unsigned short character, unsigned short color)
{
	// Bound checking if can draw
	Vec2 begin(0, 0);
	Vec2 end(width, height);
	begin.x = (x < 0) ? (x * -1) : 0;
	end.x = (dimension->x < x + width) ? end.x = dimension->x - x : end.x;
	begin.y = (y < 0) ? (y * -1) : 0;
	end.y = (dimension->y < y + height) ? end.y = dimension->y - y : end.y;
	/*x = (x < 0) ? (x * -1) : 0;
	width = (dimension->x < x + width) ? width = dimension->x - x : width;
	y = (y < 0) ? (y * -1) : 0;
	height = (dimension->y < y + height) ? height = dimension->y - y : height;*/

	//Draw
	for (int i = begin.y; i < end.y; i++)
	{
		for (int j = begin.x; j < end.x; j++)
		{
			UnsecureDraw(j + x, i + y, character, color);
		}
	}
	//for (int i = y; i < height; i++)
	//{
	//	memcpy(this->buffer + i * this->dimension->x + x, buffer, sizeof(CHAR_INFO) * width);
	//}
}

void Rect::FillArea(int x, int y, int width, int height, unsigned short character)
{
	FillArea(x, y, width, height, character, FG_COLOR_WHITE);
}

void Rect::Fill(unsigned short character, unsigned short color)
{
	for (int j = 0; j < dimension->y; j++)
	{
		for (int i = 0; i < dimension->x; i++)
		{
			Rect::UnsecureDraw(i, j, character, color);
		}
	}
}

void Rect::FillWithText(const std::string& string, unsigned short color)
{
	for (int j = 0; j < dimension->y; j++)
	{
		for (int i = 0; i < dimension->x; i++)
		{
			Rect::UnsecureDraw(i, j, string[(i + j * dimension->x) % string.length()], color);
		}
	}
}

void Rect::FillWithText(const std::string& string)
{
	FillWithText(string, FG_COLOR_WHITE);
}

//TO DO replace with memcopy for performance
void Rect::DrawRect(int x, int y, CHAR_INFO* buffer, int width, int height)
{
	// Bound checking if can draw
	Vec2 begin(0, 0);
	Vec2 end(width, height);
	begin.x = (x < 0) ? (x * -1) : 0;
	end.x = (dimension->x < x + width) ? end.x = dimension->x - x : end.x;
	begin.y = (y < 0) ? (y * -1) : 0;
	end.y = (dimension->y < y + height) ? end.y = dimension->y - y : end.y;
	/*x = (x < 0) ? (x * -1) : 0;
	width = (dimension->x < x + width) ? width = dimension->x - x : width;
	y = (y < 0) ? (y * -1) : 0;
	height = (dimension->y < y + height) ? height = dimension->y - y : height;*/

	//Draw
	for (int i = begin.y; i < end.y; i++)
	{
		for (int j = begin.x; j < end.x; j++)
		{
			UnsecureDraw(j + x, i + y, buffer[i * width + j].Char.UnicodeChar, buffer[i * width + j].Attributes);
		}
	}
	//for (int i = y; i < height; i++)
	//{
	//	memcpy(this->buffer + i * this->dimension->x + x, buffer, sizeof(CHAR_INFO) * width);
	//}
}

void Rect::DrawRect(const Rect& rect)
{
	DrawRect(rect.GetPosition().x, rect.GetPosition().y, rect.GetBuffer(), rect.GetDimension().x, rect.GetDimension().y);
}

void Rect::DrawRectTransparent(int x, int y, CHAR_INFO* buffer, int width, int height, unsigned short exeptionColor)
{
	// Bound checking if can draw
	Vec2 begin(0, 0);
	Vec2 end(width, height);
	begin.x = (x < 0) ? (x * -1) : 0;
	end.x = (dimension->x < x + width) ? end.x = dimension->x - x : end.x;
	begin.y = (y < 0) ? (y * -1) : 0;
	end.y = (dimension->y < y + height) ? end.y = dimension->y - y : end.y;
	/*x = (x < 0) ? (x * -1) : 0;
	width = (dimension->x < x + width) ? width = dimension->x - x : width;
	y = (y < 0) ? (y * -1) : 0;
	height = (dimension->y < y + height) ? height = dimension->y - y : height;*/

	//Draw
	for (int i = begin.y; i < end.y; i++)
	{
		for (int j = begin.x; j < end.x; j++)
		{
			if (buffer[i * width + j].Attributes == exeptionColor)
				continue;
			UnsecureDraw(j + x, i + y, buffer[i * width + j].Char.UnicodeChar, buffer[i * width + j].Attributes);
		}
	}
	//for (int i = y; i < height; i++)
	//{
	//	memcpy(this->buffer + i * this->dimension->x + x, buffer, sizeof(CHAR_INFO) * width);
	//}
}

void Rect::DrawRectTransparent(const Rect& rect, unsigned short exeptionColor)
{
	DrawRectTransparent(rect.GetPosition().x, rect.GetPosition().y, rect.GetBuffer(), rect.GetDimension().x, rect.GetDimension().y, exeptionColor);
}

void Rect::DrawString(int x, int y, const std::string& string)
{
	DrawString(x, y, string, FG_COLOR_WHITE);
}

void Rect::DrawString(int x, int y, const std::string& string, unsigned short color)
{
	if (y >= dimension->y || y < 0)
		return;

	int begin = (x < 0) ? (x * -1) : 0;
	int end = (dimension->x < x + (int)string.length()) ? end = dimension->x - x : string.length();

	for (int i = begin; i < end; i++)
	{
		UnsecureDraw(i + x, y, string[i], color);
	}
}

void Rect::DrawLine(int x0, int y0, int x1, int y1)
{
	DrawLine(x0, y0, x1, y1, CHARACTER_FULL, FG_COLOR_WHITE);
}

void Rect::DrawLine(int x0, int y0, int x1, int y1, unsigned short character, unsigned short color)
{
	if ((x0 < 0 && x1 < 0) || (y0 < 0 && y1 < 0) || (x0 >= dimension->x && x1 >= dimension->x) || (y0 >= dimension->y && y1 >= dimension->y))
		return;
	int dx = abs(x1 - x0);
	int sx = x0 < x1 ? 1 : -1;
	int dy = -abs(y1 - y0);
	int sy = y0 < y1 ? 1 : -1;
	int error = dx + dy;

	while (true)
	{
		Draw(x0, y0, character, color);
		if (x0 == x1 && y0 == y1) break;
		int e2 = 2 * error;
		if (e2 >= dy)
		{
			if (x0 == x1) break;
			error = error + dy;
			x0 = x0 + sx;
		}
		if (e2 <= dx)
		{
			if (y0 == y1) break;
			error = error + dx;
			y0 = y0 + sy;
		}
	}
}

void Rect::DrawPiano(int x, int y, int octaves, const PianoKeyColor& keyColor)
{
	//Draw Keys

	int keyWidth = 2;
	DrawPianoKeyWhite1(x + 1,                y + 1, CHARACTER_FULL, keyColor.whiteRelased);
	DrawPianoKeyWhite2(x + 1 + 1 * keyWidth, y + 1, CHARACTER_FULL, keyColor.whiteRelased);
	DrawPianoKeyBlack (x + 1 + 1 * keyWidth, y + 1, CHARACTER_FULL, keyColor.blackRelased);

	for (int i = 0; i < octaves; i++)
	{
		DrawPianoKeyWhite1(x + (2 + i * 7) * keyWidth + 1, y + 1, CHARACTER_FULL, keyColor.whiteRelased);
		DrawPianoKeyWhite2(x + (3 + i * 7) * keyWidth + 1, y + 1, CHARACTER_FULL, keyColor.whiteRelased);
		DrawPianoKeyBlack (x + (3 + i * 7) * keyWidth + 1, y + 1, CHARACTER_FULL, keyColor.blackRelased);
		DrawPianoKeyWhite2(x + (4 + i * 7) * keyWidth + 1, y + 1, CHARACTER_FULL, keyColor.whiteRelased);
		DrawPianoKeyBlack (x + (4 + i * 7) * keyWidth + 1, y + 1, CHARACTER_FULL, keyColor.blackRelased);
		DrawPianoKeyWhite1(x + (5 + i * 7) * keyWidth + 1, y + 1, CHARACTER_FULL, keyColor.whiteRelased);
		DrawPianoKeyWhite2(x + (6 + i * 7) * keyWidth + 1, y + 1, CHARACTER_FULL, keyColor.whiteRelased);
		DrawPianoKeyBlack (x + (6 + i * 7) * keyWidth + 1, y + 1, CHARACTER_FULL, keyColor.blackRelased);
		DrawPianoKeyWhite2(x + (7 + i * 7) * keyWidth + 1, y + 1, CHARACTER_FULL, keyColor.whiteRelased);
		DrawPianoKeyBlack (x + (7 + i * 7) * keyWidth + 1, y + 1, CHARACTER_FULL, keyColor.blackRelased);
		DrawPianoKeyWhite2(x + (8 + i * 7) * keyWidth + 1, y + 1, CHARACTER_FULL, keyColor.whiteRelased);
		DrawPianoKeyBlack (x + (8 + i * 7) * keyWidth + 1, y + 1, CHARACTER_FULL, keyColor.blackRelased);
	}
	
	DrawPianoKeyWhite1(x + (2 + octaves * 7) * keyWidth + 1, y + 1, CHARACTER_FULL, keyColor.whiteRelased);

	//Draw Border
	Draw(x, y, WIDE_SINGLE_PIPE_DOWN_RIGHT, FG_COLOR_WHITE);
	Draw(x + (3 + octaves * 7) * keyWidth + 1, y, WIDE_SINGLE_PIPE_DOWN_LEFT, FG_COLOR_WHITE);
	FillArea(x + 1, y, (3 + octaves * 7) * keyWidth, 1, WIDE_SINGLE_PIPE_HORIZONTAL, FG_COLOR_WHITE);
	FillArea(x + (3 + octaves * 7) * keyWidth + 1, y + 1, 1, 4, WIDE_SINGLE_PIPE_VERTICAL, FG_COLOR_WHITE);
	FillArea(x, y + 1, 1, 4, WIDE_SINGLE_PIPE_VERTICAL, FG_COLOR_WHITE);


}

void Rect::DrawPianoKeyWhite1(int x, int y, unsigned short character, unsigned short color)
{
	FillArea(x, y, 2, 4, character, color);
}

void Rect::DrawPianoKeyWhite2(int x, int y, unsigned short character, unsigned short color)
{
	FillArea(x, y + 2, 1, 2, character, color);
	FillArea(x + 1, y, 1, 4, character, color);
}

void Rect::DrawPianoKeyBlack(int x, int y, unsigned short character, unsigned short color)
{
	FillArea(x, y, 1, 2, character, color);
}

void Rect::DrawPianoKeyNote(int x, int y, unsigned short character, unsigned short whiteKeycolor, unsigned short blackKeycolor, unsigned char noteNumber)
{
	int keyWidth = 2;
	unsigned short key = noteNumber % 12;
	unsigned short octaveNumber = ((noteNumber - key) / 12) - 1;

	if (octaveNumber == 0 && key == 9)
	{
		DrawPianoKeyWhite1(x + keyWidth * (5 + 7 * octaveNumber), y, character, whiteKeycolor);
		return;
	}

	switch (key)
	{
	case  0: DrawPianoKeyWhite1(x + keyWidth * (0 + 7 * octaveNumber), y, character, whiteKeycolor); break; //C
	case  1: DrawPianoKeyBlack (x + keyWidth * (1 + 7 * octaveNumber), y, character, blackKeycolor); break; //C#
	case  2: DrawPianoKeyWhite2(x + keyWidth * (1 + 7 * octaveNumber), y, character, whiteKeycolor); break; //D
	case  3: DrawPianoKeyBlack (x + keyWidth * (2 + 7 * octaveNumber), y, character, blackKeycolor); break; //D#
	case  4: DrawPianoKeyWhite2(x + keyWidth * (2 + 7 * octaveNumber), y, character, whiteKeycolor); break; //E
	case  5: DrawPianoKeyWhite1(x + keyWidth * (3 + 7 * octaveNumber), y, character, whiteKeycolor); break; //F
	case  6: DrawPianoKeyBlack (x + keyWidth * (4 + 7 * octaveNumber), y, character, blackKeycolor); break; //F#
	case  7: DrawPianoKeyWhite2(x + keyWidth * (4 + 7 * octaveNumber), y, character, whiteKeycolor); break; //G
	case  8: DrawPianoKeyBlack (x + keyWidth * (5 + 7 * octaveNumber), y, character, blackKeycolor); break; //G#
	case  9: DrawPianoKeyWhite2(x + keyWidth * (5 + 7 * octaveNumber), y, character, whiteKeycolor); break; //A
	case 10: DrawPianoKeyBlack (x + keyWidth * (6 + 7 * octaveNumber), y, character, blackKeycolor); break; //A#
	case 11: DrawPianoKeyWhite2(x + keyWidth * (6 + 7 * octaveNumber), y, character, whiteKeycolor); break; //B

	default:
		break;
	}
}

void Rect::DrawNoteName(int x, int y, unsigned char noteNumber, unsigned short color)
{
	unsigned short noteName[3];
	unsigned short key = noteNumber % 12;
	unsigned short octaveNumber = ((noteNumber - key) / 12) - 1;
	switch (key)
	{
	case  0: noteName[0] = L'C'; noteName[1] = L'-';							  break;
	case  1: noteName[0] = L'C'; noteName[1] = (unsigned short)Accidental::Sharp; break;
	case  2: noteName[0] = L'D'; noteName[1] = L'-';							  break;
	case  3: noteName[0] = L'D'; noteName[1] = (unsigned short)Accidental::Sharp; break;
	case  4: noteName[0] = L'E'; noteName[1] = L'-';							  break;
	case  5: noteName[0] = L'F'; noteName[1] = L'-';							  break;
	case  6: noteName[0] = L'F'; noteName[1] = (unsigned short)Accidental::Sharp; break;
	case  7: noteName[0] = L'G'; noteName[1] = L'-';							  break;
	case  8: noteName[0] = L'G'; noteName[1] = (unsigned short)Accidental::Sharp; break;
	case  9: noteName[0] = L'A'; noteName[1] = L'-';							  break;
	case 10: noteName[0] = L'A'; noteName[1] = (unsigned short)Accidental::Sharp; break;
	case 11: noteName[0] = L'B'; noteName[1] = L'-';							  break;

	default:
		break;
	}
	noteName[2] = (octaveNumber);

	Draw(x    , y, noteName[0]);
	Draw(x + 1, y, noteName[1]);
	Draw(x + 2, y, noteName[2] + 48);
}

void Rect::UnsecureDraw(int x, int y, unsigned short character, unsigned short color)
{
	buffer[y * short(dimension->x) + x].Char.UnicodeChar = character;
	buffer[y * short(dimension->x) + x].Attributes = color;
}
