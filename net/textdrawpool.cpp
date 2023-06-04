#include "../main.h"
#include "../game/game.h"
#include "netgame.h"
#include "../gui/gui.h"

extern CGame* pGame;
extern CGUI* pGUI;

//----------------------------------------------------

CTextDrawPool::CTextDrawPool()
{
	CTouchListenerLock(this);

	int x=0;
	while(x!=MAX_TEXT_DRAWS) {
		m_pTextDraw[x] = NULL;
		m_bSlotState[x] = FALSE;
		x++;
	}
}

//----------------------------------------------------

CTextDrawPool::~CTextDrawPool()
{
	CTouchListenerLock(this);

	int x=0;
	while(x != MAX_TEXT_DRAWS) 
	{
		if(m_pTextDraw[x]) {
			delete m_pTextDraw[x];
			m_pTextDraw[x] = NULL;
			m_bSlotState[x] = FALSE;
		}
		x++;
	}
}

//----------------------------------------------------

CTextDraw* CTextDrawPool::New(uint16_t wText, TEXT_DRAW_TRANSMIT *TextDrawTransmit, char* szText)
{
	if (wText > MAX_TEXT_DRAWS || wText < 0) return nullptr;
	if(m_pTextDraw[wText]) Delete(wText);

	CTextDraw* pTextDraw = new CTextDraw(TextDrawTransmit, szText);

	if(pTextDraw)
	{
		CTouchListenerLock(this);

		m_pTextDraw[wText] = pTextDraw;
		m_bSlotState[wText] = TRUE;
		
		return pTextDraw;
	}
	return NULL;
}

//----------------------------------------------------

void CTextDrawPool::Delete(uint16_t wText)
{
	if(m_pTextDraw[wText]) 
	{
		CTouchListenerLock(this);

		delete m_pTextDraw[wText];
		m_pTextDraw[wText] = NULL;
		m_bSlotState[wText] = FALSE;
	}	
}

//----------------------------------------------------

void CTextDrawPool::Draw()
{
    int x=0;
	
	while(x != MAX_TEXT_DRAWS) {
		if(m_bSlotState[x]) m_pTextDraw[x]->Draw();
		x++;
	}
}

bool CTextDrawPool::OnTouchEvent(int type, int num, int x, int y)
{
	CTouchListenerLock(this);

	static bool bWannaClick = false;

	int id = 0;

	while (id != MAX_TEXT_DRAWS)
	{
		if (m_bSlotState[id] && m_pTextDraw[id])
		{
			switch (type)
			{
			case TOUCH_PUSH:
				if (m_pTextDraw[id]->IsXYInRect(x, y))
				{
					bWannaClick = true;
					return false;
				}
				break;

			case TOUCH_POP:
				if (m_pTextDraw[id]->IsXYInRect(x, y))
				{
					pGUI->PushToBufferedQueueTextDrawPressed((uint16_t)id);
					return false;
				}
				bWannaClick = false;
				break;

			case TOUCH_MOVE:
				break;
			}
		}
		id++;
	}

	return true;
}

//----------------------------------------------------