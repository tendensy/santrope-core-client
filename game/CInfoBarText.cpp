#include "../main.h"
#include "game.h"
#include "CInfoBarText.h"
#include "..//chatwindow.h"
#include "font.h"
#include "..//gui/gui.h"

bool CInfoBarText::m_bIsEnabled = false;

extern CGUI* pGUI;

void CInfoBarText::Draw(int fHP, int fArmour)
{
	if (!m_bIsEnabled || !pGUI)
	{
		return;
	}

	CRGBA color = CAdjustableHudColors::GetHudColor(E_HUD_ELEMENT::HUD_HP_TEXT);
	CRGBA black;
	black.A = 0;
	black.R = 0;
	black.G = 0;
	black.B = 0;
	CFont::SetColor(&color);
	CFont::SetProportional(0);
	CFont::SetBackground(0, 0);
	CFont::SetOrientation(2);
	CFont::SetRightJustifyWrap(0);
	CFont::SetFontStyle(2);
	CFont::SetDropShadowPosition(0);
	CFont::SetDropColor(&black);
	CFont::SetEdge(1);

	char buff[30];
	snprintf(buff, sizeof(buff), "%dHP", fHP);
	uint16_t buff_16[30];
	CFont::AsciiToGxtChar(buff, buff_16);

	CFont::SetScale((float)CAdjustableHudScale::GetElementScale(E_HUD_ELEMENT::HUD_HP_TEXT).X / 100.0f, 0);
	CFont::PrintString(pGUI->ScaleX(CAdjustableHudPosition::GetElementPosition(E_HUD_ELEMENT::HUD_HP_TEXT).X),
		pGUI->ScaleY(CAdjustableHudPosition::GetElementPosition(E_HUD_ELEMENT::HUD_HP_TEXT).Y), buff_16);

	color = CAdjustableHudColors::GetHudColor(E_HUD_ELEMENT::HUD_ARMOR_TEXT);
	CFont::SetColor(&color);
	CFont::SetProportional(0);
	CFont::SetBackground(0, 0);
	CFont::SetOrientation(2);
	CFont::SetRightJustifyWrap(0);
	CFont::SetFontStyle(2);
	CFont::SetDropShadowPosition(0);
	CFont::SetDropColor(&black);
	CFont::SetEdge(1);

	snprintf(buff, sizeof(buff), "%d", fArmour);
	CFont::AsciiToGxtChar(buff, buff_16);

	CFont::SetScale((float)CAdjustableHudScale::GetElementScale(E_HUD_ELEMENT::HUD_ARMOR_TEXT).X / 100.0f, 0);
	CFont::PrintString(pGUI->ScaleX(CAdjustableHudPosition::GetElementPosition(E_HUD_ELEMENT::HUD_ARMOR_TEXT).X),
		pGUI->ScaleY(CAdjustableHudPosition::GetElementPosition(E_HUD_ELEMENT::HUD_ARMOR_TEXT).Y), buff_16);

}

void CInfoBarText::SetEnabled(bool bEnable)
{
	m_bIsEnabled = bEnable;
}
