#include <stdlib.h>

#include "main.h"
#include "game/game.h"
#include "net/netgame.h"
#include "gui/gui.h"
#include "dialog.h"
#include "vendor/imgui/imgui_internal.h"
#include "keyboard.h"

extern CGUI* pGUI;
extern CGame* pGame;
extern CNetGame* pNetGame;
extern CKeyBoard* pKeyBoard;

char szDialogInputBuffer[100];
char utf8DialogInputBuffer[100 * 3];

#define SIZE_BUTTON_X	180.0f

CDialogWindow::CDialogWindow()
{
	m_bIsActive = false;
	m_putf8Info = nullptr;
	m_pszInfo = nullptr;
	m_KeyIdPressed = -1;
	m_bRendered = false;
	ImGui::GetStyle().Colors[ImGuiCol_Button] = ImVec4(0.13f, 0.13f, 0.13f, 0.95f);
	ImGui::GetStyle().Colors[ImGuiCol_ButtonActive] = ImVec4(0.65f, 0.30f, 0.30f, 1.00f);
	ImGui::GetStyle().Colors[ImGuiCol_ButtonHovered] = ImVec4(0.8f, 0.30f, 0.30f, 1.00f);
	ImGui::GetStyle().WindowMinSize = ImVec2(pGUI->ScaleX(500.0f), ImGui::GetStyle().WindowMinSize.y);

	m_fButtonSizeX = pGUI->ScaleX(SIZE_BUTTON_X);
	m_fButtonSizeY = pGUI->ScaleY(80.0f);
}

CDialogWindow::~CDialogWindow()
{

}

void CDialogWindow::Show(bool bShow)
{
	if (pGame)
		pGame->FindPlayerPed()->TogglePlayerControllable(!bShow);

	m_bIsActive = bShow;

	if (bShow != true)
	{
		m_KeyIdPressed = -1;
	}
}

void CDialogWindow::Clear()
{
	if (m_putf8Info)
	{
		free(m_putf8Info);
		m_putf8Info = nullptr;
	}

	if (m_pszInfo)
	{
		free(m_pszInfo);
		m_pszInfo = nullptr;
	}

	m_bIsActive = false;
	m_byteDialogStyle = 0;
	m_wDialogID = -1;
	m_utf8Title[0] = 0;
	m_utf8Button1[0] = 0;
	m_utf8Button2[0] = 0;


	m_fSizeX = -1.0f;
	m_fSizeY = -1.0f;

	memset(szDialogInputBuffer, 0, 100);
	memset(utf8DialogInputBuffer, 0, 100 * 3);
}

void CDialogWindow::SetInfo(char* szInfo, int length)
{
	if (!szInfo || !length) return;

	if (m_putf8Info)
	{
		free(m_putf8Info);
		m_putf8Info = nullptr;
	}

	if (m_pszInfo)
	{
		free(m_pszInfo);
		m_pszInfo = nullptr;
	}

	m_putf8Info = (char*)malloc((length * 3) + 1);
	if (!m_putf8Info)
	{
		Log("CDialog::SetInfo: can't allocate memory");
		return;
	}

	m_pszInfo = (char*)malloc((length * 3) + 1);
	if (!m_pszInfo)
	{
		Log("CDialog::SetInfo: can't allocate memory");
		return;
	}

	cp1251_to_utf8(m_putf8Info, szInfo);

	// =========
	char szNonColorEmbeddedMsg[4200];
	int iNonColorEmbeddedMsgLen = 0;

	for (size_t pos = 0; pos < strlen(szInfo) && szInfo[pos] != '\0'; pos++)
	{
		if (pos + 7 < strlen(szInfo))
		{
			if (szInfo[pos] == '{' && szInfo[pos + 7] == '}')
			{
				pos += 7;
				continue;
			}
		}

		szNonColorEmbeddedMsg[iNonColorEmbeddedMsgLen] = szInfo[pos];
		iNonColorEmbeddedMsgLen++;
	}

	szNonColorEmbeddedMsg[iNonColorEmbeddedMsgLen] = 0;
	// ========

	cp1251_to_utf8(m_pszInfo, szNonColorEmbeddedMsg);
}

bool ProcessInlineHexColor(const char* start, const char* end, ImVec4& color);

void TextWithColors(const char* fmt, ...)
{
	char tempStr[4096];

	va_list argPtr;
	va_start(argPtr, fmt);
	vsnprintf(tempStr, sizeof(tempStr), fmt, argPtr);
	va_end(argPtr);
	tempStr[sizeof(tempStr) - 1] = '\0';

	bool pushedColorStyle = false;
	const char* textStart = tempStr;
	const char* textCur = tempStr;
	while (textCur < (tempStr + sizeof(tempStr)) && *textCur != '\0')
	{
		if (*textCur == '{')
		{
			// Print accumulated text
			if (textCur != textStart)
			{
				ImGui::TextUnformatted(textStart, textCur);
				ImGui::SameLine(0.0f, 0.0f);
			}

			// Process color code
			const char* colorStart = textCur + 1;
			do
			{
				++textCur;
			} while (*textCur != '\0' && *textCur != '}');

			// Change color
			if (pushedColorStyle)
			{
				ImGui::PopStyleColor();
				pushedColorStyle = false;
			}

			ImVec4 textColor;
			if (ProcessInlineHexColor(colorStart, textCur, textColor))
			{
				ImGui::PushStyleColor(ImGuiCol_Text, textColor);
				pushedColorStyle = true;
			}

			textStart = textCur + 1;
		}
		else if (*textCur == '\n')
		{
			// Print accumulated text an go to next line
			ImGui::TextUnformatted(textStart, textCur);
			textStart = textCur + 1;
		}

		++textCur;
	}

	if (textCur != textStart)
		ImGui::TextUnformatted(textStart, textCur);
	else
		ImGui::NewLine();

	if (pushedColorStyle)
		ImGui::PopStyleColor();
}

void DialogWindowInputHandler(const char* str)
{
	if (!str || *str == '\0') return;
	strcpy(szDialogInputBuffer, str);
	cp1251_to_utf8(utf8DialogInputBuffer, str);
}

void CDialogWindow::Render()
{
	m_bRendered = false;
	if (!m_bIsActive || !m_putf8Info) return;

	ImGuiIO& io = ImGui::GetIO();
	ImVec2 buttsize;
	ImVec2 incurSize;
	buttsize.x = ImGui::CalcTextSize("QWERTYUIOPAS").x;
	incurSize.x = ImGui::CalcTextSize("QWERTYUIOPASDFGHJKLZXCVBN").x;

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(16, 16));

	ImGui::Begin("dialog", nullptr,
		ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings);
	m_bRendered = true;
	std::vector<std::string> lines;

	TextWithColors(m_utf8Title);
	ImGui::ItemSize(ImVec2(0, pGUI->GetFontSize() / 2 + 2.75));

	switch (m_byteDialogStyle)
	{
	case DIALOG_STYLE_MSGBOX:
		ImGui::GetStyle().ButtonTextAlign = ImVec2(0.5f, 0.5f);
		TextWithColors(m_putf8Info);
		ImGui::ItemSize(ImVec2(0, pGUI->GetFontSize() / 2));
		break;

	case DIALOG_STYLE_INPUT:
		ImGui::GetStyle().ButtonTextAlign = ImVec2(0.5f, 0.5f);
		TextWithColors(m_putf8Info);
		ImGui::ItemSize(ImVec2(0, 50));

		if (ImGui::Button(utf8DialogInputBuffer, ImVec2(ImGui::GetWindowWidth() - 2.0f * ImGui::GetStyle().WindowPadding.x, 50)))
		{
			pKeyBoard->Open(&DialogWindowInputHandler);
		}

		ImGui::ItemSize(ImVec2(0, pGUI->GetFontSize() / 2));
		break;
	case DIALOG_STYLE_PASSWORD:
		ImGui::GetStyle().ButtonTextAlign = ImVec2(0.5f, 0.5f);

		TextWithColors(m_putf8Info);
		ImGui::ItemSize(ImVec2(0, 50));

		if (strlen(utf8DialogInputBuffer) <= 0)
		{
			if (ImGui::Button(" ", ImVec2(incurSize.x * 1.5, 50)))
			{
				pKeyBoard->Open(&DialogWindowInputHandler);
			}
		}
		else {
			if (ImGui::Button("**********", ImVec2(incurSize.x * 1.5, 50)))
			{
				pKeyBoard->Open(&DialogWindowInputHandler);
			}
		}

		ImGui::ItemSize(ImVec2(0, pGUI->GetFontSize() / 2));
		break;

	case DIALOG_STYLE_LIST:

		ImGui::GetStyle().WindowPadding = ImVec2(8, 8);

		std::string line = m_putf8Info;

		while (line.find('\n') != -1)
		{
			unsigned short int posV = line.find('\n');
			if (posV <= 0) break;

			std::string buffer;

			buffer.assign(line, 0, posV);
			line.erase(0, posV + 1);

			if (buffer[0] != '\0')
				lines.push_back(buffer);

			if (posV <= 0) break;
		}

		if (line[0] != '\0') lines.push_back(line);

		ImGui::GetStyle().ButtonTextAlign = ImVec2(0.1f, 0.5f); // left | center
		ImGui::GetStyle().ItemSpacing = ImVec2(0.0f, pGUI->GetFontSize() / 2);
		unsigned int curSize = lines.size();

		for (int i = 0; i < curSize; i++)
		{
			ImGuiStyle style;
			style.Colors[ImGuiCol_Button] = ImGui::GetStyle().Colors[ImGuiCol_Button];

			if (m_KeyIdPressed == i) ImGui::GetStyle().Colors[ImGuiCol_Button] = ImGui::GetStyle().Colors[ImGuiCol_ButtonActive];

			ImVec2 curpos = ImGui::GetCursorPos();
			std::string szName;

			for (int x = 0; x < i; x++) szName.insert(szName.begin(), ' ');

			ImVec2 butSize;
			if ((ImGui::CalcTextSize(m_pszInfo).x * 1.4f + pGUI->GetFontSize() * 3) > (buttsize.x) * 2)
				butSize.x = ImGui::CalcTextSize(m_pszInfo).x * 1.4f;
			else butSize.x = (buttsize.x) * 2 + pGUI->GetFontSize();

			if (ImGui::Button((char*)szName.c_str(), ImVec2(butSize.x, pGUI->GetFontSize() * 2.0f)))
			{
				if (m_KeyIdPressed == i)
				{
					ImGui::GetStyle().Colors[ImGuiCol_Button] = ImVec4(0.13f, 0.13f, 0.13f, 0.95f);
					Show(false);
					if (pNetGame) {
						if (m_byteDialogStyle == DIALOG_STYLE_LIST)
						{
							pNetGame->SendDialogResponse(m_wDialogID, 1, i, (char*)lines[i].c_str());
						}
					}
					ImGui::SetWindowSize(ImVec2(-1, -1));
					ImVec2 size = ImGui::GetWindowSize();
					ImGui::SetWindowPos(ImVec2(((io.DisplaySize.x - size.x) / 2), ((io.DisplaySize.y - size.y) / 2)));
					ImGui::End();

					ImGui::PopStyleVar();
					return;
				}

				strcpy(szDialogInputBuffer, lines[i].c_str());
				m_KeyIdPressed = i;
			}

			ImVec2 newxtcurpos = ImGui::GetCursorPos();
			ImGui::SetCursorPos(ImVec2(curpos.x + pGUI->GetFontSize(), curpos.y + pGUI->GetFontSize() / 2));
			TextWithColors(lines[i].c_str());
			ImGui::SetCursorPos(newxtcurpos);

			ImGui::GetStyle().Colors[ImGuiCol_Button] = style.Colors[ImGuiCol_Button];
		}

		ImGui::GetStyle().ItemSpacing = ImVec2(0.0f, 1.0f);
		ImGui::GetStyle().ButtonTextAlign = ImVec2(0.5f, 0.5f);

		ImGui::Dummy(ImVec2(1, pGUI->GetFontSize()));
		ImGui::GetStyle().WindowPadding = ImVec2(pGUI->GetFontSize(), pGUI->GetFontSize());
		break;
	}

	ImVec2 button1Size = ImVec2(ImGui::CalcTextSize(m_utf8Button1).x * 1.5f, pGUI->GetFontSize() * 2);
	ImVec2 button2Size = ImVec2(ImGui::CalcTextSize(m_utf8Button2).x * 1.5f, pGUI->GetFontSize() * 2);
	ImVec2 buttonSize = ImMax(button1Size, button2Size);

	ImGui::SetCursorPosX((ImGui::GetWindowWidth() - 2.2f * pGUI->ScaleX(SIZE_BUTTON_X) + ImGui::GetStyle().ItemSpacing.x) / 2);

	if (m_utf8Button1[0] != 0)
	{
		if (ImGui::Button(m_utf8Button1, ImVec2(m_fButtonSizeX, m_fButtonSizeY)))
		{
			if (pNetGame)
				pNetGame->SendDialogResponse(m_wDialogID, 1, m_KeyIdPressed, szDialogInputBuffer);

			Show(false);
		}
	}

	ImGui::SameLine(0, pGUI->GetFontSize());

	if (m_utf8Button2[0] != 0)
	{
		if (ImGui::Button(m_utf8Button2, ImVec2(m_fButtonSizeX, m_fButtonSizeY)))
		{
			if (pNetGame)
				pNetGame->SendDialogResponse(m_wDialogID, 0, -1, szDialogInputBuffer);

			Show(false);
		}
	}

	ImGui::SetWindowSize(ImVec2(-1, -1));
	ImVec2 size = ImGui::GetWindowSize();
	ImGui::SetWindowPos(ImVec2(((io.DisplaySize.x - size.x) / 2), ((io.DisplaySize.y - size.y) / 2)));
	ImGui::End();

	ImGui::PopStyleVar();
}