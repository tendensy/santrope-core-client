#include "../main.h"
#include "font.h"

void CFont::AsciiToGxtChar(const char* ascii, uint16_t* gxt)
{
	return (( void (*)(const char*, uint16_t*))(g_libGTASA+0x532D00+1))(ascii, gxt);
}

void CFont::SetOrientation(int alignment)
{
	((int (*)(char))(g_libGTASA + 0x005339E8 + 1))((char)alignment);
}

void CFont::SetColor(CRGBA* color)
{
	((int (*)(CRGBA*))(g_libGTASA + 0x005336F4 + 1))(color);
}

void CFont::SetEdge(short value)
{
	((int (*)(char))(g_libGTASA + 0x0053394C + 1))((char)value);
}

void CFont::SetBackground(bool enable, bool includeWrap)
{
	((int (*)(char, char))(g_libGTASA + 0x00533988 + 1))(enable, includeWrap);
}

void CFont::SetWrapx(float value)
{
	((int (*)(float))(g_libGTASA + 0x0053381C + 1))(value);
}

void CFont::SetBackgroundColor(CRGBA* color)
{
	((int (*)(CRGBA*))(g_libGTASA + 0x005339A4 + 1))(color);
}

void CFont::SetScale(float w, float h)
{
	((int (*)(float))(g_libGTASA + 0x00533694 + 1))(w);
}

void CFont::SetFontStyle(short style)
{
	((int (*)(char))(g_libGTASA + 0x00533748 + 1))((char)style);
}

void CFont::SetProportional(bool on)
{
	((int (*)(char))(g_libGTASA + 0x00533970 + 1))((char)on);
}

void CFont::PrintString(float X, float Y, uint16_t* sz)
{
	Font_PrintString(X, Y, sz);
}

void CFont::SetRightJustifyWrap(float v)
{
	((int (*)(float))(g_libGTASA + 0x0053384C + 1))(v);
}

void CFont::SetDropShadowPosition(char a)
{
	((int (*)(char))(g_libGTASA + 0x005338DC + 1))(a);
}

void CFont::SetDropColor(CRGBA* color)
{
	((int (*)(CRGBA*))(g_libGTASA + 0x0053387C + 1))(color);
}

void Font_PrintString(float X, float Y, uint16_t* sz)
{
	((int (*)(float, float, uint16_t*))(g_libGTASA + 0x005353B4 + 1))(X, Y, sz);
}

//----------------------------------------------------------

void Font_SetColor(CRGBA* rgba)
{
	((int (*)(CRGBA *))(g_libGTASA + 0x005336F4 + 1))(rgba);
}

//-----------------------------------------------------------

void Font_SetDropColor(CRGBA* rgba)
{
	((int (*)(CRGBA *))(g_libGTASA + 0x0053387C + 1))(rgba);
}

//-----------------------------------------------------------

void Font_SetOutline(int pos)
{
	//0053394C
	((int (*)(char))(g_libGTASA + 0x0053394C + 1))((char)pos);
}

//-----------------------------------------------------------

void Font_Unk12(int unk)
{
	//005339D0
	((int (*)(char))(g_libGTASA + 0x005339D0 + 1))((char)unk);
}

//-----------------------------------------------------------

void Font_SetScale(float X, float Y)
{
	((void (*)(float, float))(g_libGTASA + 0x00533694 + 1))(X, Y);
	//00533694
	//00533694
	/*((int (*)(char))(g_libGTASA + 0x005339D0 + 1))((char)unk);
	_asm mov eax, 0x719380
	_asm jmp eax*/
}

//-----------------------------------------------------------
// 0 center 1 left 2 right

void Font_SetJustify(int just)
{
	((int (*)(char))(g_libGTASA + 0x005339E8 + 1))((char)just);
	//005339E8
}

//-----------------------------------------------------------

void Font_SetFontStyle(int style)
{
	//00533748
	uint8_t* korean = (uint8_t*)(g_libGTASA + 0x00988382);
	uint8_t* russia = (uint8_t*)(g_libGTASA + 0x00988383);
	uint8_t* japanese = (uint8_t*)(g_libGTASA + 0x00988384);
	uint8_t oKorean, oRussia, oJapanese;
	oKorean = *korean;
	oRussia = *russia;
	oJapanese = *japanese;
	*korean = 0;
	*russia = 0;
	*japanese = 0;
	((int (*)(char))(g_libGTASA + 0x00533748 + 1))((char)style);
	*korean = oKorean;
	*russia = oRussia;
	*japanese = oJapanese;
}

//-----------------------------------------------------------

void Font_SetProportional(int prop)
{
	//00533970
	((int (*)(char))(g_libGTASA + 0x00533970 + 1))((char)prop);
}

//-----------------------------------------------------------

void Font_SetRightJustifyWrap(float wrap)
{
	((int (*)(float))(g_libGTASA + 0x0053384C + 1))(wrap);
	//0053384C
}

//-----------------------------------------------------------

void Font_UseBox(int use, int unk)
{
	//00533988
	((int (*)(char, char))(g_libGTASA + 0x00533988 + 1))(use, unk);
}

//-----------------------------------------------------------

void Font_UseBoxColor(CRGBA* rgba)
{
	//005339A4
	((int (*)(CRGBA *))(g_libGTASA + 0x005339A4 + 1))(rgba);
}
//-----------------------------------------------------------

void Font_SetLineWidth(float width)
{
	//0053381C
	((int (*)(float))(g_libGTASA + 0x0053381C + 1))(width);
}

//-----------------------------------------------------------

void Font_SetLineHeight(float height)
{
	((int (*)(float))(g_libGTASA + 0x00533834 + 1))(height);
	//00533834
}

//-----------------------------------------------------------

void Font_SetShadow(int shadow)
{
	//005338DC
	((int (*)(char))(g_libGTASA + 0x005338DC + 1))((char)shadow);
}

//-----------------------------------------------------------

void Font_UnkConv(char* sz, int param1, int param2, int param3, int param4, int param5, int param6, char* buf)
{
	//004D213C
	((int (*)(char*, int, int, int, int, int, int, char*))(g_libGTASA + 0x004D213C + 1))(sz, param1, param2, param3, param4, param5, param6, buf);
}

//-----------------------------------------------------------

void Font_UnkConv2(char* sz)
{
	/*_asm mov eax, 0x69E160
	_asm jmp eax*/
}