#include "../main.h"
#include "../game/game.h"
#include "netgame.h"

CPlayerPool::CPlayerPool()
{
	m_pLocalPlayer = new CLocalPlayer();

	for(PLAYERID playerId = 0; playerId < MAX_PLAYERS; playerId++)
	{
		m_bPlayerSlotState[playerId] = false;
		m_pPlayers[playerId] = nullptr;
	}
}

CPlayerPool::~CPlayerPool()
{
	delete m_pLocalPlayer;
	m_pLocalPlayer = nullptr;

	for(PLAYERID playerId = 0; playerId < MAX_PLAYERS; playerId++)
		Delete(playerId, 0);
}

#include "..//chatwindow.h"
#include "..//game/game.h"
#include "..//net/netgame.h"
#include "..//CSettings.h"
extern CSettings* pSettings;
extern CGame* pGame;
extern CChatWindow* pChatWindow;
extern CNetGame* pNetGame;
uint32_t bProcessedfsaf = 0;
int g_iStatusDriftChanged = 0;
bool CPlayerPool::Process()
{
	bProcessedfsaf = 0;
	MAKE_PROFILE(test, test_time)
	for(PLAYERID playerId = 0; playerId < MAX_PLAYERS; playerId++)
	{
		if (m_bPlayerSlotState[playerId])
		{
			m_pPlayers[playerId]->Process();
			CPlayerPed* pPed = m_pLocalPlayer->GetPlayerPed();
			CPlayerPed* pPedRemote = m_pPlayers[playerId]->GetPlayerPed();
			if (pPed && pPedRemote && pPed->IsInVehicle())
			{
				if (pPed->GetGtaVehicle() == pPedRemote->GetGtaVehicle())
				{

					//pChatWindow->AddDebugMessage("Playerid %d is in the same vehicle", playerId);
				}
				else
				{
					if (pPedRemote)
					{

					}
				}
			}

			else
			{
				if (pPedRemote)
				{

				}
			}
		}
	}

	m_pLocalPlayer->Process();
	CPlayerPed* pPed = m_pLocalPlayer->GetPlayerPed();
	if (pPed)
	{
		MATRIX4X4 mat;
		MATRIX4X4 cam;
		pPed->GetMatrix(&mat);

		pNetGame->GetStreamPool()->PostListenerMatrix(&mat);

		if (pGame->GetCamera())
		{
			pGame->GetCamera()->GetMatrix(&cam);
		}
		//PLAYER_VOICE_POS pos;
		VECTOR up;
		VECTOR front;
		//pos.pos.X = mat.pos.X;
		//pos.pos.Y = mat.pos.Y;
		//pos.pos.Z = mat.pos.Z;

		if (m_pLocalPlayer->IsSpectating())
		{
			if (m_pLocalPlayer->m_byteSpectateType == SPECTATE_TYPE_PLAYER)
			{
				CRemotePlayer* pSpectated = m_pPlayers[m_pLocalPlayer->m_SpectateID];
				if (pSpectated)
				{
					if (pSpectated->GetPlayerPed())
					{
						pSpectated->GetPlayerPed()->GetMatrix(&mat);
						//pos.pos.X = mat.pos.X;
						//pos.pos.Y = mat.pos.Y;
						//pos.pos.Z = mat.pos.Z;
					}
				}
			}
			if (m_pLocalPlayer->m_byteSpectateType == SPECTATE_TYPE_VEHICLE)
			{
				CVehicle* pSpectated = pNetGame->GetVehiclePool()->GetAt(m_pLocalPlayer->m_SpectateID);
				if (pSpectated)
				{
					pSpectated->GetMatrix(&mat);
					//pos.pos.X = mat.pos.X;
					//pos.pos.Y = mat.pos.Y;
					//pos.pos.Z = mat.pos.Z;
				}

			}
		}

		up.X = cam.up.X;
		up.Y = cam.up.Y;
		up.Z = cam.up.Z;

		front.X = cam.at.X*(-1.0f);
		front.Y = cam.at.Y * (-1.0f);
		front.Z = cam.at.Z * (-1.0f);
	}
	LOG_PROFILE(test, test_time)
	return true;
}

bool CPlayerPool::New(PLAYERID playerId, char *szPlayerName, bool IsNPC)
{
	m_pPlayers[playerId] = new CRemotePlayer();

	if(m_pPlayers[playerId])
	{
		strcpy(m_szPlayerNames[playerId], szPlayerName);
		m_pPlayers[playerId]->SetID(playerId);
		m_pPlayers[playerId]->SetNPC(IsNPC);
		m_bPlayerSlotState[playerId] = true;

		return true;
	}

	return false;
}


bool CPlayerPool::Delete(PLAYERID playerId, uint8_t byteReason)
{
	if(!GetSlotState(playerId) || !m_pPlayers[playerId])
		return false;

	if(GetLocalPlayer()->IsSpectating() && GetLocalPlayer()->m_SpectateID == playerId)
		GetLocalPlayer()->ToggleSpectating(false);

	m_bPlayerSlotState[playerId] = false;
	delete m_pPlayers[playerId];
	m_pPlayers[playerId] = nullptr;
	return true;
}

PLAYERID CPlayerPool::FindRemotePlayerIDFromGtaPtr(PED_TYPE * pActor)
{
	CPlayerPed *pPlayerPed;

	for(PLAYERID playerId = 0; playerId < MAX_PLAYERS; playerId++)
	{
		if(true == m_bPlayerSlotState[playerId])
		{
			pPlayerPed = m_pPlayers[playerId]->GetPlayerPed();

			if(pPlayerPed) {
				PED_TYPE *pTestActor = pPlayerPed->GetGtaActor();
				if((pTestActor != NULL) && (pActor == pTestActor)) // found it
					return m_pPlayers[playerId]->GetID();
			}
		}
	}

	return INVALID_PLAYER_ID;	
}