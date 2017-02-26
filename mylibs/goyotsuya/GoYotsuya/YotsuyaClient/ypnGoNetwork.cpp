#include "ypnGoNetwork.h"

// ---------------------------
cypnGoNetwork::cypnGoNetwork(void)
	: cyPrivateNetwork()
{
	m_hUserMutex = yiMutexCreate(0);
}

// ---------------------------
cypnGoNetwork::~cypnGoNetwork(void)
{
}

//// ---------------------------
Bool cypnGoNetwork::SendTextMessage(HandleUser hUser,  const char* buffer)
{
	sGoUserInfo* info = (sGoUserInfo*)hUser;
	return SendToUser(info->index, (int)eGNPMSG_SEND_TEXT_MESSAGE, (void*)buffer, strlen(buffer)+1);	
}

static unsigned int xReverseCondition(unsigned int condition)
{
	unsigned int rconditon = condition;
	if (IAM_BLACK(rconditon)==TRUE)
	{
		SET_IAM_WHITE(rconditon);
	}
	else
	{
		SET_IAM_BLACK(rconditon);
	}

	return rconditon;
}

// ---------------------------
Bool cypnGoNetwork::SendRequestPlay(HandleUser hUser, unsigned int con1, unsigned int con2)
{
	int buffer[2]={xReverseCondition(con1), con2};

	sGoUserInfo* info = (sGoUserInfo*)hUser;
	return SendToUser(info->index, (int)eGNPMSG_REQ_PLAY, (char*)buffer, sizeof(buffer));
}

// ---------------------------
Bool cypnGoNetwork::SendReplyPlay(HandleUser hUser, unsigned int con1, unsigned int con2)
{
	unsigned int buffer[2]={(con1==0xFFFFFFFF)?0xFFFFFFFF:xReverseCondition(con1), con2};

	sGoUserInfo* info = (sGoUserInfo*)hUser;
	
	return SendToUser(info->index, (int)eGNPMSG_REPLY_PLAY, (char*)buffer, sizeof(buffer));
}

// ---------------------------
unsigned int cypnGoNetwork::SendRequestPlayStart(HandleUser hUser, unsigned int* con1, unsigned int con2)
{
	if (IS_NIGIRI(*con1)==TRUE)
	{
		Bool bIsBlack = rand()%2;

		if (bIsBlack==True)
		{
			SET_IAM_BLACK(*con1);
		}
		else
		{
			SET_IAM_WHITE(*con1);
		}
	}

	unsigned int id =cyiTime::GetLocalTime();
	unsigned int condition = xReverseCondition(*con1);
	unsigned int buffer[4] = {id, condition, con2};

	sGoUserInfo* info = (sGoUserInfo*)hUser;
	info->id = id;

	yiTrace("SendRequestPlayStart go id is %d", id);
	if (SendToUser(info->index, (int)eGNPMSG_REQ_PLAY_START, (char*)buffer, sizeof(buffer))==FALSE)
	{
		return 0;
	}

	return id;
}

// ---------------------------
Bool cypnGoNetwork::SendRequestPlayEnd(HandleUser hUser, unsigned int id, ePlayEndStatus status)
{
	unsigned int buffer[2]={id, status};
	sGoUserInfo* info = (sGoUserInfo*)hUser;
	return SendToUser(info->index, (int)eGNPMSG_REQ_PLAY_END, (char*)buffer, sizeof(buffer));
}

// ---------------------------
Bool cypnGoNetwork::SendRequestPlayEndOk(HandleUser hUser, unsigned int id, ePlayEndStatus status, Bool bIsOK)
{
	unsigned int buffer[3]={id, status, bIsOK};
	sGoUserInfo* info = (sGoUserInfo*)hUser;
	return SendToUser(info->index, (int)eGNPMSG_REQ_PLAY_END_OK, (char*)buffer, sizeof(buffer));
}

// ---------------------------
Bool cypnGoNetwork::SendRequestCalculateEnd(HandleUser hUser, unsigned int id)
{
	sGoUserInfo* info = (sGoUserInfo*)hUser;
	return SendToUser(info->index, (int)eGNPMSG_REQ_CALCULATE_END, &id, sizeof(id));
}

// ---------------------------
Bool cypnGoNetwork::SendRequestCalculateEndOk(HandleUser hUser, unsigned int id, Bool bIsOk)
{
	unsigned int buffer[2]={id, bIsOk};
	sGoUserInfo* info = (sGoUserInfo*)hUser;
	return SendToUser(info->index, (int)eGNPMSG_REQ_CALCULATE_END_OK, buffer, sizeof(buffer));
}

// ---------------------------
Bool cypnGoNetwork::SendStone(HandleUser hUser, unsigned int id, stone_t stone)
{
	unsigned int buffer[2]={id, stone};
	sGoUserInfo* info = (sGoUserInfo*)hUser;
	return SendToUser(info->index, (int)eGNPMSG_SEND_STONE, (char*)buffer, sizeof(buffer));
}


static void* MakeNotifyMsg(HandleUser hUser, void* data, int size)
{
	char* p = (char*)yialloc(size+sizeof(hUser));
	HandleUser* x = (HandleUser*)p;
	x[0]=hUser;
	memcpy(&x[1], data, size);
	return x;

}

// ---------------------------
Bool cypnGoNetwork::OnMessage(socket_t sock, int index, int id, void* buffer)
{
	if (cyPrivateNetwork::OnMessage(sock, index, id, buffer)==False)
	{	
		return False;
	}

	if (m_hUserList[index]==0)
	{
		return False;
	}

	switch(id)
	{
		case eGNPMSG_SEND_TEXT_MESSAGE :
			NotifyMessage(id, MakeNotifyMsg(m_hUserList[index], buffer, strlen((const char*)buffer)+1));
			break;
		case eGNPMSG_REQ_PLAY:
		{
			NotifyMessage(id, MakeNotifyMsg(m_hUserList[index], buffer, 12));
			break;
		}
		case eGNPMSG_REPLY_PLAY:
		{
			NotifyMessage(id, MakeNotifyMsg(m_hUserList[index], buffer, 8));
			break;
		}
		case eGNPMSG_REQ_PLAY_START:
		{
			sGoUserInfo* info = (sGoUserInfo*)m_hUserList[index];
			unsigned int* _buffer = (unsigned int*)buffer;
			info->id = _buffer[0];
				
			NotifyMessage(id, MakeNotifyMsg(m_hUserList[index], buffer, 8));
			break;
		}
		case eGNPMSG_REQ_PLAY_END:
		{
			NotifyMessage(id, MakeNotifyMsg(m_hUserList[index], buffer, 8));
			break;
		}
		case eGNPMSG_REQ_PLAY_END_OK:
		{
			NotifyMessage(id, MakeNotifyMsg(m_hUserList[index], buffer, 12));
			break;
		}
		case eGNPMSG_REQ_CALCULATE_END:
		{
			NotifyMessage(id, MakeNotifyMsg(m_hUserList[index], buffer, 4));
			break;
		}
		case eGNPMSG_REQ_CALCULATE_END_OK:
		{
			NotifyMessage(id, MakeNotifyMsg(m_hUserList[index], buffer, 8));
			break;
		}

		case eGNPMSG_SEND_STONE:
		{
			yiTrace("SendStone Recved\n");
			NotifyMessage(id, MakeNotifyMsg(m_hUserList[index], buffer, 8));
			break;
		}
	
	}

	yifree(buffer);

	return True;
}	

// ---------------------------
void cypnGoNetwork::OnAddUser(socket_t sock, int index)
{
	//sypnUserInfo* info = (sypnUserInfo*)sock->pExtraData;
	sGoUserInfo info = {sock, index, 0};

	yiMutexLock(m_hUserMutex);
	m_hUserList[index] = (HandleUser)yitoheap(&info, sizeof(info));
	yiMutexUnlock(m_hUserMutex);
}

// ---------------------------
void cypnGoNetwork::OnRemoveUser(socket_t sock, int index)
{
	yiMutexLock(m_hUserMutex);
	yifree((void*)m_hUserList[index]);
	m_hUserList[index]=0;
	yiMutexUnlock(m_hUserMutex);
}


// ---------------------------
HandleUser cypnGoNetwork::FindUser(const char* pszUserName, unsigned int id, unsigned int go_id)
{
	yiMutexLock(m_hUserMutex);
	for (int i=0; i<MAX_CONNECT_USER_CNT; i++)
	{
		if (m_hUserList[i]==0)
		{
			continue;
		}
		sGoUserInfo* s = (sGoUserInfo*)m_hUserList[i];

		if (go_id!=0)
		{
			if (s->id==go_id)
			{
				yiMutexUnlock(m_hUserMutex);
				return m_hUserList[i];
			}
			continue;
		}

		sypnUserInfo* info = (sypnUserInfo* )s->sock->pExtraData;
		if (info==0)
		{
			continue;
		}


		if (id!=0)
		{
			if (info->id==id)
			{
				yiMutexUnlock(m_hUserMutex);
				return m_hUserList[i];
			}
			continue;
		}


		if (pszUserName && strcmp(info->szUserName, pszUserName)==0)
		{
			yiMutexUnlock(m_hUserMutex);
			return m_hUserList[i];
		}
	}

	yiMutexUnlock(m_hUserMutex);
	return 0;
}


// ---------------------------
HandleUser cypnGoNetwork::IndexToUser(int i)
{
	return m_hUserList[i];
}

// ---------------------------
void cypnGoNetwork::SetGoId(HandleUser hUser, unsigned int id)
{
	sGoUserInfo* s = (sGoUserInfo*)hUser;
	s->id = id;
}

// ---------------------------
unsigned int cypnGoNetwork::GetUserId(HandleUser hUser)
{
	sGoUserInfo* s = (sGoUserInfo*)hUser;

	if (s==0)
	{
		return 0;
	}

	if (s->sock==0)
	{
		return 0;
	}

	if (s->sock->pExtraData==0)
	{
		return 0;
	}
	return ((sypnUserInfo*)s->sock->pExtraData)->id;
}

// ---------------------------
const char* cypnGoNetwork::GetUserName(HandleUser hUser)
{
	sGoUserInfo* s = (sGoUserInfo*)hUser;

	if (s==0)
	{
		return 0;
	}

	if (s->sock==0)
	{
		return 0;
	}

	if (s->sock->pExtraData==0)
	{
		return 0;
	}
	return (const char*)((sypnUserInfo*)s->sock->pExtraData)->szUserName;
}