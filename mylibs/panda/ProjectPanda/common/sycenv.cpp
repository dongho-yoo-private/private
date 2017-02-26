#include "yistd.h"
#include "yiList.h"
#include "yipsr.h"
#include "sycenv.h"
#include <stdlib.h>
#include <string.h>

// ---------------------------
const char* csycEnv::GetLocalPortName(const char* pszServerName, char* out_path)
{
	sprintf(out_path, "%s/%s.port", getenv(	"YOTSUYA_DIR_LOCAL_PORT"), pszServerName);
	return (const char*)out_path;
}

// ---------------------------
const char* csycEnv::GetUserRoot()
{
	return getenv("YOTSUYA_DIR_SVRDATA_USER");
}

// ---------------------------
const char* csycEnv::GetGroupRoot()
{
	return getenv("YOTSUYA_DIR_SVRDATA_GROUP");
}

// ---------------------------
const char* csycEnv::GetPrivateRoot()
{
	return getenv("YOTSUYA_PRIVATE_ROOT");
}

// ---------------------------
const char* csycEnv::GetRecordRoot()
{
	return getenv("YOTSUYA_RECORD_ROOT");
}

// ---------------------------
const char* csycEnv::GetShadowRoot()
{
	return getenv("YOTSUYA_SHADOW_ROOT");
}
// ---------------------------
const char* csycEnv::GetEmailRoot()
{
	return getenv("YOTSUYA_DIR_SVRDATA_EMAIL");
}

// ---------------------------
Bool csycEnv::GetServerDefinition(const char* pszServerName, sServerInformation* server, sServerInformation* local)
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
		yiTrace("file load error (%s)\n", buffer);
		return False;
	}

	if (yiPsrSplit(pFile, "\n", lines)==False)
	{
		yiFileUnload(pFile);
		return False;
	}
	yiFileUnload(pFile);

	yiTrace("++++++++++ [%s] ++++++++++\n", pszServerName);

	for (pNode=lines.GetNodeTopFrom(0); pNode!=0; pNode=pNode->np)
	{
		char buffer[256];
		char* p = (char*)pNode->ref;

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
				yiTrace("++++++++++ Local Information ++++++++++\n");
				continue;
			}
			else if (strcmp(buffer, "Server")==0)
			{
				current_mode_local = False;
				pInfo = server;
				memset(pInfo, 0, sizeof(*pInfo));
				yiTrace("++++++++++ Server Information ++++++++++ \n");
				continue;
			}
			else
			{
				printf("Unknown mode %s\n", buffer);
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
			yiTrace("timeout: %d\n", n);
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
			yiTrace("maxpacket: %d\n", n);
			pInfo->nMaxPacketSize=n;
			continue;
		} 
		else if (strncmp(p, "port", 4)==0)
		{
			if (yiPsrFromCpy(buffer, p, "=")==0)
			{
				return False;
			}

			yiTrace("port: %s\n", buffer);
			if (current_mode_local == True)
			{
				yistrcpy(pInfo->szServerAddress, buffer);
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
				yiTrace("type: %s\n", buffer);
			}
			else
			{
				pInfo->type=SERVER_TYPE_LIMITED;
				yiTrace("type: %s\n", buffer);
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
				yiTrace("packet: %s\n", buffer);
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
			yiTrace("maxc: %d\n", n);
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
			yiTrace("maxa: %d\n", n);
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
			yiTrace("listen: %d\n", n);
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
			yiTrace("extra%d: %d\n", index, n);
			pInfo->extra[index] = n;
			continue;
		}
	}		

	return True;
}




