#include "yistd.h"
#include "yiList.h"
#include "yipsr.h"
#include "svryenv.h"
#include <stdlib.h>
#include <string.h>

// ---------------------------
const char* csvryEnv::GetLocalPortName(const char* pszServerName, char* out_path)
{
	sprintf(out_path, "%s/%s.port", getenv("YOTSUYA_DIR_LOCAL_PORT"), pszServerName);
	return (const char*)out_path;
}

// ---------------------------
const char* csvryEnv::GetUserRoot()
{
	return getenv("YOTSUYA_DIR_SVRDATA_USER");
}

// ---------------------------
const char* csvryEnv::GetGroupRoot()
{
	return getenv("YOTSUYA_DIR_SVRDATA_GROUP");
}

// ---------------------------
const char* csvryEnv::GetPrivateRoot()
{
	return getenv("YOTSUYA_PRIVATE_ROOT");
}

// ---------------------------
const char* csvryEnv::GetRecordRoot()
{
	return getenv("YOTSUYA_RECORD_ROOT");
}

// ---------------------------
const char* csvryEnv::GetShadowRoot()
{
	return getenv("YOTSUYA_SHADOW_ROOT");
}
// ---------------------------
const char* csvryEnv::GetEmailRoot()
{
	return getenv("YOTSUYA_DIR_SVRDATA_EMAIL");
}

// ---------------------------
Bool csvryEnv::GetServerDefinition(const char* pszServerName, sServerInformation* server, sServerInformation* local)
{
	char buffer[256];
	char* pFile; 
	cyiList lines(yifree);
	sServerInformation* pInfo = local;
	Bool current_mode_local = False;
	cyiNode* pNode;

	sprintf(buffer, "%s/%s.def", getenv("YOTSUYA_DIR_SVRDEF"), pszServerName);
	pFile = (char*)yiFileLoad(buffer, 0);	
	
	if (pFile==0)
	{
		YIDEBUG("file load error (%s)\n", buffer);
		return False;
	}

	if (yiPsrSplit(pFile, "\n", lines)==False)
	{
		yiFileUnload(pFile);
		return False;
	}
	yiFileUnload(pFile);

	YIDEBUG("++++++++++ [%s] ++++++++++\n", pszServerName);

	for (pNode=lines.GetNodeTopFrom(0); pNode!=0; pNode=pNode->np)
	{
		char buffer[256];
		char* p = (char*)pNode->ref;

		//YIDEBUG("line: %s, %x, %c\n", p, (unsigned long)p[0], p[0]);

		if (p==0)
		{
			continue;
		}

		if (p[0]=='#')
			continue;

		if (p[0]=='[')
		{
			if (yiPsrUntilCpy(buffer, &p[1], "]")==0)
			{
				//delete pLine;
				return False;
			}


			if (strcmp(buffer, "Local")==0)
			{
				current_mode_local = True;
				pInfo = local;
				memset(pInfo, 0, sizeof(*pInfo));
				YIDEBUG("++++++++++ Local Information ++++++++++\n");
				continue;
			}
			else if (strcmp(buffer, "Server")==0)
			{
				current_mode_local = False;
				pInfo = server;
				memset(pInfo, 0, sizeof(*pInfo));
				YIDEBUG("++++++++++ Server Information ++++++++++ \n");
				continue;
			}
			else
			{
				YIWARN("Unknown mode %s\n", buffer);
				continue;
			}
		}

		if (pInfo==NULL)
		{
			continue;
		}

		if (strncmp(p, "timeout", 7)==0)
		{
			int n;
			if (yiPsrFromCpy(buffer, p, "=")==0)
			{
				return False;
			}
			n = atoi(buffer);
			YIDEBUG("timeout: %d\n", n);
			pInfo->nLimitedTypeTimeOut=n;
			continue;
		}
		else if (strncmp(p, "maxpacket", 9)==0)
		{
			int n;
			if (yiPsrFromCpy(buffer, p, "=")==0)
			{
				return False;
			}
			n = atoi(buffer);
			YIDEBUG("maxpacket: %d\n", n);
			pInfo->nMaxPacketSize=n;
			continue;
		} 
		else if (strncmp(p, "port", 4)==0)
		{
			if (yiPsrFromCpy(buffer, p, "=")==0)
			{
				return False;
			}

			YIDEBUG("port: %s\n", buffer);
			if (current_mode_local == True)
			{
				//yistrcpy(pInfo->szServerAddress, buffer, 0);
				strcpy(pInfo->szServerAddress, buffer);
			}
			else
			{
				int n = atoi(buffer);

				if (n==0)
				{
					//delete pLine;
					return False;
				}

				pInfo->nPortNo=n;
			}

			continue;
		}
		else if (strncmp(p, "type", 4)==0)
		{
			if (yiPsrFromCpy(buffer, p, "=")==0)
			{
				return False;
			}

			if (strncmp(buffer, "normal", 6)==0)
			{
				pInfo->type=SERVER_TYPE_NORMAL;
				YIDEBUG("type: %s\n", buffer);
			}
			else
			{
				pInfo->type=SERVER_TYPE_LIMITED;
				YIDEBUG("type: %s\n", buffer);
			}
			continue;
		}
		else if (strncmp(p, "packet", 6)==0)
		{
			if (yiPsrFromCpy(buffer, p, "=")==0)
			{
				return False;
			}

			if (strncmp(buffer, "clear", 5)==0)
			{
				pInfo->bIsPacketClear=True;
				pInfo->security=0;
				YIDEBUG("packet: %s\n", buffer);
			}
			continue;
		}
		else if (strncmp(p, "maxc", 4)==0)
		{
			int n;
			if (yiPsrFromCpy(buffer, p, "=")==0)
			{
				return False;
			}
			n = atoi(buffer);
			YIDEBUG("maxc: %d\n", n);
			pInfo->nMaxConnections=n;

			continue;
		}
		else if (strncmp(p, "maxa", 4)==0)
		{
			if (yiPsrFromCpy(buffer, p, "=")==0)
			{
				return False;
			}
			int n = atoi(buffer);
			YIDEBUG("maxa: %d\n", n);
			pInfo->nMaxArrayCount=n;
			continue;
		}
		else if (strncmp(p, "listen", 6)==0)
		{
			if (yiPsrFromCpy(buffer, p, "=")==0)
			{
				return False;
			}
			int n = atoi(buffer);
			YIDEBUG("listen: %d\n", n);
			pInfo->nMaxListenWaitCnt=n;
			continue;
		}
		else if (strncmp(p, "extra", 5)==0)
		{
			if (yiPsrFromCpy(buffer, p, "=")==0)
			{
				return False;
			}

			int index = (int)(p[5]-'0');
			int n = atoi(buffer);
			YIDEBUG("extra%d: %d\n", index, n);
			pInfo->extra[index] = n;
			continue;
		}
	}		

	return True;
}




