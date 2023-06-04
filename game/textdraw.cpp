/*

SA:MP Multiplayer Modification
Copyright 2004-2005 SA:MP Team

Version: $Id: textdraw.cpp,v 1.4 2008-04-16 08:54:17 kyecvs Exp $

*/

#include "../main.h"
#include "game.h"
#include "font.h"

#include <vector>
#include <string>
#include <sstream>
#include "../chatwindow.h"
extern CChatWindow* pChatWindow;
extern CGame *pGame;

//char ProvideTmp[1024];

CTextDraw::CTextDraw(TEXT_DRAW_TRANSMIT *TextDrawTransmit, char* szText)
{
	memset(&m_TextDrawData,0,sizeof(TEXT_DRAW_DATA));
	m_pSprite = nullptr;
	// Set standard parameters
	m_TextDrawData.fLetterWidth = TextDrawTransmit->fLetterWidth;
	m_TextDrawData.fLetterHeight = TextDrawTransmit->fLetterHeight;
	m_TextDrawData.dwLetterColor = TextDrawTransmit->dwLetterColor;
	m_TextDrawData.byteUnk12 = 0;
	m_TextDrawData.byteCentered = TextDrawTransmit->byteCenter;
	m_TextDrawData.byteBox = TextDrawTransmit->byteBox;
	m_TextDrawData.fLineWidth = TextDrawTransmit->fLineWidth;
	m_TextDrawData.fLineHeight = TextDrawTransmit->fLineHeight;
	m_TextDrawData.dwBoxColor = TextDrawTransmit->dwBoxColor;
	m_TextDrawData.byteProportional = TextDrawTransmit->byteProportional;
	m_TextDrawData.dwBackgroundColor = TextDrawTransmit->dwBackgroundColor;
	m_TextDrawData.byteShadow = TextDrawTransmit->byteShadow;
	m_TextDrawData.byteOutline = TextDrawTransmit->byteOutline;
	m_TextDrawData.byteAlignLeft = TextDrawTransmit->byteLeft;
	m_TextDrawData.byteAlignRight = TextDrawTransmit->byteRight;
	m_TextDrawData.dwStyle = TextDrawTransmit->byteStyle;
	m_TextDrawData.fX = TextDrawTransmit->fX;
	m_TextDrawData.fY = TextDrawTransmit->fY;
	m_TextDrawData.byteSelectabe = TextDrawTransmit->byteSelectabe;
	m_TextDrawData.dwParam1 = 0xFFFFFFFF;
	m_TextDrawData.dwParam2 = 0xFFFFFFFF;
	
	strncpy((char*)m_szText, (const char*)szText, MAX_TEXT_DRAW_LINE);
	m_szText[MAX_TEXT_DRAW_LINE - 1] = '\0';

	if (m_TextDrawData.dwStyle == 4)
	{
		m_pSprite = new CSprite2d();
		m_pSprite->m_pRwTexture = nullptr;
		std::vector<std::string> strings;
		std::istringstream f((char*)m_szText);
		std::string s;
		while (std::getline(f, s, ':')) 
		{
			strings.push_back(s);
		}
		if (strings.size() == 2)
		{
			m_pSprite->m_pRwTexture = (RwTexture*)LoadTextureFromDB("gui", strings[1].c_str());
			strcpy(m_szTexture, strings[1].c_str());
		}
		else
		{
			pChatWindow->AddDebugMessage("An error occured when trying load sprite with name %s", m_szText);
		}
	}
}

CTextDraw::~CTextDraw()
{
	if (m_TextDrawData.dwStyle == 4 && m_pSprite)
	{
		delete m_pSprite;
	}
}

bool CTextDraw::IsXYInRect(int x, int y)
{
	if (!m_TextDrawData.byteSelectabe) return false;
	int iScreenWidth, iScreenHeight;
	float fVertHudScale, fHorizHudScale;

	iScreenWidth = pGame->GetScreenWidth();
	iScreenHeight = pGame->GetScreenHeight();
	fVertHudScale = pGame->GetHudVertScale();
	fHorizHudScale = pGame->GetHudHorizScale();

	float fLineHeight = iScreenWidth * fHorizHudScale * m_TextDrawData.fLineHeight;

	float fPS2Height = 448.0f;
	float fPS2Width = 640.0f;
	float fScriptY = m_TextDrawData.fY;
	float fScriptX = m_TextDrawData.fX;
	float fUseX, fUseY;

	float fLineWidth;
	if (m_TextDrawData.dwStyle != 4)
	{
		fLineWidth = m_TextDrawData.fLineWidth - m_TextDrawData.fX;
		fLineWidth = iScreenWidth * fHorizHudScale * fLineWidth;
	}
	else if (m_TextDrawData.dwStyle == 4)
	{
		fLineWidth = iScreenWidth * fHorizHudScale * m_TextDrawData.fLineWidth;
	}

	fUseY = iScreenHeight - ((fPS2Height - fScriptY) * (iScreenHeight * fVertHudScale));
	fUseX = iScreenWidth - ((fPS2Width - fScriptX) * (iScreenWidth * fHorizHudScale));
	return (x >= fUseX && x <= fUseX + fLineWidth &&
		y >= fUseY && y <= fUseY + fLineHeight);
}

void CTextDraw::SetText(uint8_t* pStr, uint16_t wLen)
{
	memset((void*)m_szText, 0, MAX_TEXT_DRAW_LINE - 1);
	memcpy((void*)m_szText, (void*)pStr, wLen);
}

CRGBA DWORD2RGBA(uint32_t dwColor)
{
	CRGBA tmp;

	tmp.R = dwColor & 0xFF; dwColor >>= 8;
	tmp.G = dwColor & 0xFF; dwColor >>= 8;
	tmp.B = dwColor & 0xFF; dwColor >>= 8;
	tmp.A = dwColor & 0xFF; /* dwColor >>= 8; */

	return tmp;
}

void CTextDraw::Draw()
{
	if (!m_szText || !strlen((const char*)m_szText)) return;

	memset(m_szDrawString, 0, MAX_TEXT_DRAW_LINE * 4 * sizeof(uint16_t));
	CFont::AsciiToGxtChar((const char*)m_szText, m_szDrawString);

	int iScreenWidth, iScreenHeight;
	float fVertHudScale, fHorizHudScale;

	iScreenWidth = pGame->GetScreenWidth();
	iScreenHeight = pGame->GetScreenHeight();
	fVertHudScale = pGame->GetHudVertScale();
	fHorizHudScale = pGame->GetHudHorizScale();

	float fScaleY = iScreenHeight * fVertHudScale * m_TextDrawData.fLetterHeight * 0.5f * 2.0f;
	float fScaleX = iScreenWidth * fHorizHudScale * m_TextDrawData.fLetterWidth * 2.0f;


	if (m_TextDrawData.dwStyle == 4 && m_pSprite)
	{
		if (!m_pSprite->m_pRwTexture) return;
		CRGBA letterColor = DWORD2RGBA(m_TextDrawData.dwLetterColor);

		float fPS2Height = 448.0f;
		float fPS2Width = 640.0f;
		float fScriptY = m_TextDrawData.fY;
		float fScriptX = m_TextDrawData.fX;
		float fUseX, fUseY;

		fUseY = fScriptY * iScreenHeight * fVertHudScale;
		fUseX = iScreenWidth - ((fPS2Width - fScriptX) * (iScreenWidth * fHorizHudScale));

		float fLineWidth = iScreenWidth * fHorizHudScale * m_TextDrawData.fLineWidth;

		float fLineHeight = iScreenHeight * fVertHudScale * m_TextDrawData.fLineHeight;

		if (strcmp(m_pSprite->m_pRwTexture->name, m_szTexture))
		{
			RwTexture* rwOldTex = m_pSprite->m_pRwTexture;
			m_pSprite->m_pRwTexture = (RwTexture*)LoadTextureFromDB("gui", m_szTexture);
			RwTextureDestroy(rwOldTex);
		}

		m_pSprite->Draw(fUseX, fUseY, fLineWidth, fLineHeight, letterColor);
		return;
	}


	Font_SetScale(fScaleX, fScaleY);
	CRGBA letterColor = DWORD2RGBA(m_TextDrawData.dwLetterColor);
	Font_SetColor(&letterColor);
	Font_Unk12(0);

	if (m_TextDrawData.byteAlignRight) Font_SetJustify(2);
	else if (m_TextDrawData.byteCentered) Font_SetJustify(0);
	else Font_SetJustify(1);

	float fLineWidth = iScreenWidth * fHorizHudScale * m_TextDrawData.fLineWidth;
	Font_SetLineWidth(fLineWidth);

	float fLineHeight = iScreenHeight * fVertHudScale * m_TextDrawData.fLineHeight;
	Font_SetLineHeight(fLineHeight);

	Font_UseBox(m_TextDrawData.byteBox, 0);
	CRGBA boxColor = DWORD2RGBA(m_TextDrawData.dwBoxColor);
	Font_UseBoxColor(&boxColor);
	Font_SetProportional(m_TextDrawData.byteProportional);
	CRGBA backgroundColor = DWORD2RGBA(m_TextDrawData.dwBackgroundColor);
	Font_SetDropColor(&backgroundColor);

	if (m_TextDrawData.byteOutline) {
		Font_SetOutline(m_TextDrawData.byteOutline);
	}
	else {
		Font_SetShadow(m_TextDrawData.byteShadow);
	}

	Font_SetFontStyle(m_TextDrawData.dwStyle);

	float fPS2Height = 448.0f;
	float fPS2Width = 640.0f;
	float fScriptY = m_TextDrawData.fY;
	float fScriptX = m_TextDrawData.fX;
	float fUseX, fUseY;

	fUseY = fScriptY * iScreenHeight * fVertHudScale;
	fUseX = iScreenWidth - ((fPS2Width - fScriptX) * (iScreenWidth * fHorizHudScale));

	Font_PrintString(fUseX, fUseY, m_szDrawString);
	Font_SetOutline(0);
}
