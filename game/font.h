#pragma once

struct CRGBA
{
	uint8_t R;
	uint8_t G;
	uint8_t B;
	uint8_t A;
};

void Font_PrintString(float X, float Y, uint16_t* sz);
void Font_SetColor(CRGBA* rgba);
void Font_SetDropColor(CRGBA* rgba);
void Font_SetOutline(int pos);
void Font_Unk12(int unk);
void Font_SetScale(float X, float Y);
void Font_SetJustify(int just);
void Font_SetFontStyle(int style);
void Font_SetProportional(int prop);
void Font_SetRightJustifyWrap(float wrap);
void Font_UseBox(int use, int unk);
void Font_UseBoxColor(CRGBA* rgba);
void Font_SetLineWidth(float width);
void Font_SetLineHeight(float height);
void Font_SetShadow(int shadow);
void Font_UnkConv(char* sz, int param1, int param2, int param3, int param4, int param5, int param6, char* buf);
void Font_UnkConv2(char* sz);

class CFont
{
public:
	static void AsciiToGxtChar(const char* ascii, uint16_t* gxt);
	static void SetOrientation(int alignment);//005339E8
	static void SetColor(CRGBA* color);
	static void SetEdge(short value);
	static void SetBackground(bool enable, bool includeWrap);
	static void SetWrapx(float value);
	static void SetBackgroundColor(CRGBA* color);
	static void SetScale(float w, float h);
	static void SetFontStyle(short style);
	static void SetProportional(bool on);
	static void PrintString(float X, float Y, uint16_t* sz);

	static void SetRightJustifyWrap(float v);
	static void SetDropShadowPosition(char a);
	static void SetDropColor(CRGBA* color);
};