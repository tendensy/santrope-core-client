#pragma once

/*

SA:MP Multiplayer Modification
Copyright 2004-2005 SA:MP Team

Version: $Id: textdraw.h,v 1.3 2008-02-11 22:30:15 kyecvs Exp $

*/

#define MAX_TEXT_DRAW_LINE 256

//-----------------------------------------------------------

#pragma pack(1)
typedef struct _TEXT_DRAW_DATA
{
	float fLetterWidth;		// -4
	float fLetterHeight;	// 0
	uint32_t dwLetterColor;	// 4
	uint8_t byteUnk12;			// 8
	uint8_t byteCentered;		// 9
	uint8_t byteBox;			// 10
	PADDING(_pad15, 1);		// 11
	float fLineWidth;		// 12
	float fLineHeight;		// 16
	uint32_t dwBoxColor;		// 20
	uint8_t byteProportional;	// 24
	uint32_t dwBackgroundColor;// 25-29
	uint8_t byteShadow;		// 29
	uint8_t byteOutline;		// 30
	uint8_t byteAlignLeft;		// 31
	uint8_t byteAlignRight;	// 32
	uint8_t byteSelectabe;
	PADDING(_pad37, 2);		// 33,34,35
	uint32_t dwStyle;			// 36
	float fX;				// 40
	float fY;				// 44
	char cGXT[8];			// 48
	uint32_t dwParam1;			// 60-64
	uint32_t dwParam2;			// 64-68
} TEXT_DRAW_DATA;

//-----------------------------------------------------------

#pragma pack(1)
typedef struct _TEXT_DRAW_TRANSMIT
{
	union
	{
		uint8_t byteFlags;
		struct
		{
			uint8_t byteBox : 1;
			uint8_t byteLeft : 1;
			uint8_t byteRight : 1;
			uint8_t byteCenter : 1;
			uint8_t byteProportional : 1;
			uint8_t bytePadding : 3;
		};
	};
	float fLetterWidth;
	float fLetterHeight;
	uint32_t dwLetterColor;
	float fLineWidth;
	float fLineHeight;
	uint32_t dwBoxColor;
	uint8_t byteShadow;
	uint8_t byteOutline;
	uint32_t dwBackgroundColor;
	uint8_t byteStyle;
	uint8_t byteSelectabe;
	float fX;
	float fY;
	uint16_t wModelID;
	VECTOR rot;
	float fZoom;
	uint16_t wColor1;
	uint16_t wColor2;
} TEXT_DRAW_TRANSMIT;

//----------------------------------------------------

#pragma pack(1)
#include "sprite2d.h"
class CTextDraw
{
private:

	uint8_t			m_szText[MAX_TEXT_DRAW_LINE];
	uint16_t		m_szDrawString[MAX_TEXT_DRAW_LINE * 4];
	char			m_szTexture[MAX_TEXT_DRAW_LINE];
	TEXT_DRAW_DATA  m_TextDrawData;
	CSprite2d* m_pSprite;
public:
	CTextDraw(TEXT_DRAW_TRANSMIT *TextDrawTransmit, char* szText);
	~CTextDraw();
	
	bool IsXYInRect(int x, int y);

	uint8_t* GetText() { return m_szText; };
	void SetText(uint8_t* pStr, uint16_t wLen);

	void Draw();
};


//----------------------------------------------------
