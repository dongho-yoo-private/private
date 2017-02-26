#include "svryuser.h"
#include "sycUser.h"
#include "yiHashFunc.h"
#include "yiEncoder.h"
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>

// --------------------------
Bool csvryuser::OnPreRegisterSuccess(int sock, const char* pszEMailAddress, unsigned long n)
{
	char* p = (char*)yialloc(YI_MAX_EMAIL_LENGTH+(sizeof(int)<<1));
	if (!p)
		return False;

	memcpy(p, pszEMailAddress, n);
	p[n]=0;

	srand(time(0));
	int rand1 = rand();
	int rand2 = rand();

	__set8to32__(p, YI_MAX_EMAIL_LENGTH, rand1);
	__set8to32__(p, YI_MAX_EMAIL_LENGTH+sizeof(int), rand2);


	m_PreRegisterEmailList.lock();
	if (m_PreRegisterEmailList.addref(p)==false)
	{
		m_PreRegisterEmailList.unlock();
		yifree(p);
		return False;
	}

	m_PreRegisterEmailList.unlock();

	ssyNormalPacket packet;

	packet.data.param[0]=0;
	packet.data.param[1]=rand1;
	packet.data.param[2]=rand2;
	write(sock, &packet, sizeof(packet));

	yiTrace("csvryuser::OnPreRegisterSuccess() success (%08x, %08x)\n", rand1, rand2);
	return True;
}

// --------------------------
Bool csvryuser::OnPreRegister(int sock, ssyNormalPacket& packet)
{
	char szEmailAddress[128];

	if (read(sock, szEmailAddress, packet.data.param[0])!=packet.data.param[0])
	{
		return False;		
	}

	szEmailAddress[packet.data.param[0]]=0;

	m_PreRegisterEmailList.lock();

	if (m_PreRegisterEmailList.count()!=0)
	{
		for (cyiNode* p = m_PreRegisterEmailList.GetNodeTopFrom(0); p!=0; p=p->np)
		{
			if (p->ref==0)
			{
				SYSLOG(,"??? list is null\n");
				continue;
			}
			
			if (memcmp(p->ref, szEmailAddress, packet.data.param[0])==0)
			{
				m_PreRegisterEmailList.unlock();
				packet.data.param[0]=0xFFFFFFFE;
				write(sock, &packet, sizeof(packet));
				return False;
				
			}
		}
	}

	m_PreRegisterEmailList.unlock();

	int rand1 = yihf_additional(szEmailAddress, 8);
	int rand2 = yihf_encoded_additional(szEmailAddress, 8);

	yiTrace("OnPreRegister() rand1:%d, rand2:%d\n", rand1, rand2);
	if (IsExistMail(__8to32__(rand1, rand2, 0, 0),  szEmailAddress)!=-1)
	{
		packet.data.param[0]=0xFFFFFFFD;
		write(sock, &packet, sizeof(packet));
		return False;
	}

	if (OnPreRegisterSuccess(sock, szEmailAddress, packet.data.param[0])==False)
	{
		packet.data.param[0]=0xFFFFFFFF;
		write(sock, &packet, sizeof(packet));
		return False;
	}

	return True;
}

// --------------------------
Bool csvryuser::OnRequestEmailAddress(int sock, unsigned long rand1, unsigned long rand2)
{

	yiTrace("csvryuser::OnRequestEmailAddress() rand1:%08x, rand2:%08x\n", rand1, rand2);
	m_PreRegisterEmailList.lock();
	for (cyiNode* p=m_PreRegisterEmailList.GetNodeTopFrom(0); p!=0; p=p->np)
	{
		if (p->ref==0)
		{
			SYSLOG(,"??? list is null\n");
			continue;
		}

		char* pmail = (char*)p->ref;
		yiTrace("%08x, %08x\n", __p8to32__(pmail, YI_MAX_EMAIL_LENGTH), 
				__p8to32__(pmail, YI_MAX_EMAIL_LENGTH+sizeof(int)));
		if (__p8to32__(pmail, YI_MAX_EMAIL_LENGTH) == rand1 && 
			__p8to32__(pmail, YI_MAX_EMAIL_LENGTH+sizeof(int))==rand2)
		{
			
			m_PreRegisterEmailList.unlock();

			char buffer[YI_MAX_EMAIL_LENGTH+sizeof(int)];
			int n = strlen((const char*)p->ref);
			__set8to32__(buffer, 0, n);
			memcpy(&buffer[sizeof(int)], p->ref, n);
			buffer[sizeof(int)+n]=0;

			write(sock, buffer, n+sizeof(int)+1);

			yiTrace("Email address..... %s\n", p->ref);

			return True;
		}
	}

	int res = 0xFFFFFFFF;
	write(sock, &res, sizeof(int));
	m_PreRegisterEmailList.unlock();
	return False;	
}

// --------------------------
Bool csvryuser::OnRegister(int sock, ssyNormalPacket& packet)
{
	ssycUserBasicInfo* info = (ssycUserBasicInfo*)yialloc(sizeof(ssycUserBasicInfo));

	if (read(sock, info, sizeof(ssycUserBasicInfo))!=sizeof(ssycUserBasicInfo))
	{
		yifree(info);
		packet.data.param[0]=0xFFFFFFFF;
		write(sock, &packet, sizeof(packet));
		return False;
	}

	yiTrace("++++++++ register user info ++++++++ \n");
	yiTrace("name:%s\n", info->name);
	yiTrace("email:%s\n", info->email_addr);
	yiTrace("password:%s\n", info->password);
	yiTrace("erea:%d\n", info->address);
	yiTrace("sex:%s\n", __biton__(info->exfield[0], 26) ? "Mail":"Femail");
	yiTrace("birth:%4d/%2d/%2d\n", __year__(info->birthday), __month__(info->birthday), __day__(info->birthday));
	yiTrace("grade:%d\n", (info->exfield[0] >>27));
	yiTrace("++++++++ register user info ++++++++ \n");


	Bool bIsEmailFind=False;
	m_PreRegisterEmailList.lock();

	if (m_PreRegisterEmailList.count()!=0)
	{
		for (cyiNode* p = m_PreRegisterEmailList.GetNodeTopFrom(0); p!=0; p=p->np)
		{
			if (p->ref==0)
			{
				SYSLOG(,"??? list is null\n");
				continue;
			}
			
			if (strncmp((const char*)p->ref, info->email_addr, YI_MAX_EMAIL_LENGTH)!=0)
			{
				continue;
			}

			m_PreRegisterEmailList.remove(p, yifree);	
			bIsEmailFind=True;
		}
	}

	m_PreRegisterEmailList.unlock();

	if (bIsEmailFind==False)
	{
		yifree(info);
		packet.data.param[0]=0xFFFFFFFE;
		write(sock, &packet, sizeof(packet));
		return False;
	}

	char szFileName[256];
	int rand1 = yihf_additional(info->email_addr, 8);
	int rand2 = yihf_encoded_additional(info->email_addr, 8);

	if (IsExistMail(__8to32__(rand1, rand2, 0, 0) , info->email_addr)!=-1)
	{
		yifree(info);
		packet.data.param[0]=0xFFFFFFFA;
		write(sock, &packet, sizeof(packet));
		return False;
	}

	int randuser1 = yihf_additional(info->name, 8);
	int randuser2 = yihf_encoded_additional(info->name, 8);
	int randuser3 = yihf_general(info->name, 8);

	
	if (IsExistName(__8to32__(randuser1 , randuser2 , randuser3, 0) , info->name)!=-1)
	{
		yifree(info);
		packet.data.param[0]=0xFFFFFFF9;
		write(sock, &packet, sizeof(packet));
		return False;
	}

	char* pEmail = yistrheap(info->email_addr, YI_MAX_EMAIL_LENGTH);
	int   mailId = m_EmailHash.AddRefData(__8to32__(rand1, rand2, 0, 0), pEmail);

	if (mailId==-1)
	{
		yifree(info);
		packet.data.param[0]=0xFFFFFFFB;
		write(sock, &packet, sizeof(packet));
		return False;
	}


	sprintf(szFileName, "%s/%03d/%03d/%d", csycEnv::GetEmailRoot(), rand1, rand2, mailId);
	if (CreditWrite(szFileName, pEmail, YI_MAX_EMAIL_LENGTH, 4)==False)
	{
		yifree(info);
		packet.data.param[0]=0xFFFFFFFC;
		write(sock, &packet, sizeof(packet));
		return False;
	}
	yiTrace("mail id: %d, %d, %d, (%08x)\n", rand1, rand2, mailId, __8to32__(rand1, rand2, 0, mailId));


	int userId = m_UserDataHash.AddRefData(__8to32__(randuser1, randuser2, randuser3, 0), info);

	if (userId==-1)
	{
		yifree(info);
		packet.data.param[0]=0xFFFFFFFB;
		write(sock, &packet, sizeof(packet));
		return False;
	}

	packet.data.param[1] = __8to32__(randuser1, randuser2, randuser3, userId);

	info->id = packet.data.param[1];

	yiTrace("user id: %d, %d, %d, %d, (%08x)\n", randuser1, randuser2, randuser3, packet.data.param[1]);

	sprintf(szFileName, "%s/%03d/%03d/%03d/%d", csycEnv::GetUserRoot(), randuser1, randuser2, randuser3, userId);
	if (CreditWrite(szFileName, info, sizeof(*info), 4)==False)
	{
		yifree(info);
		packet.data.param[0]=0xFFFFFFFC;
		write(sock, &packet, sizeof(packet));
		return False;
	}

	packet.data.param[0]=0;
	write(sock, &packet, sizeof(packet));

	return True;	
}

// --------------------------
Bool csvryuser::OnRequestUserInfo(int sock, ssyNormalPacket& packet)
{
/*	char name[16];
	if (packet.data.param[3]==0)
	{
		char buffer[32];
		int len = (YI_MAX_NAME_LENGTH<<1)+sizeof(int);
		if (read(sock, buffer, len)!=len)
		{
			packet.data.param[0]=0xFFFFFFFF;
			cyiPacket::WriteNormalPacket(packet);
			return False;
		}

		if (cyiEncoder::dstr(name, buffer)==False)
		{
			packet.data.param[0]=0xFFFFFFFE;
			cyiPacket::write(packet);
			return False;
		}
	}

*/	
}


// --------------------------
Bool csvryuser::OnLogIn(int sock, ssyNormalPacket& packet, void** pExtraData)
{
	sLoginExtraData buffer;
	char name[YI_MAX_NAME_LENGTH];
	Bit32 id = __8to32__(packet.data.param[0], packet.data.param[1], packet.data.param[2], packet.data.param[3]);

	if (read(sock, buffer, sizeof(buffer))!=sizeof(buffer))
	{
		SYSLOG(,"read error\n");
		return False;	
	}

	if (cyiEncode::dstr(buffer.encoded_id)==0)
	{
		SYSLOG(,"encoding read error\n");
		return False;
	}

	if (packet.data.param[3]==0)
	{
		int cnt=0;
		cyiList* pList = m_UserDataHash.GetList(id);

		if (pList==NULL)
		{
			packet.data.param[0]=eULEC_INVALID_PASSWORD;
			goto LOGIN_ERROR;
		}

		for (cyiNode* p = pList->GetNodeTopFrom(0); p!=0; p=p->np)
		{
			sycUserInfo* info = (ssycUserBasicInfo* )p->ref;
			++cnt;
			if (strncmp(info->name, buffer.encoded_id, YI_MAX_NAME_LENGTH)!=0)
				continue;

			packet.data.param[3]=cnt;
			*pExtraData = (void*)info;
			break;
		}

		if (packet.data.param[3]==0)
		{
			return False;
		}

		m_UserDataHash.ReleaseList(pList);
	}
	else
	{
		info = (ssycUserBasicInfo* )m_UserDataHash.GetData(id);

		if (info==0)
		{
			packet.data.param[0]=eULEC_INVALID_USERNAME;
			goto LOGIN_ERROR;
		}
		if (strncmp(info->name, buffer.encoded_id, YI_MAX_NAME_LENGTH)!=0)
		{
			packet.data.param[0]=eULEC_INVALID_USERNAME;
			goto LOGIN_ERROR;
		}
	}

	if (cyiEncode::dstr(buffer.encoded_password)!=0)
	{
		SYSLOG(,"encoding read error\n");
		packet.data.param[0]=eULEC_INVALID_PASSWORD;
		goto LOGIN_ERROR;
	}

	// とりあえず、ここまできたらユーザの認証は終了。
	//packet.data.param[0]=info->id;

	//if (packet.data.param[3]!=m_current_version)
	//{
	//	return False;		
	//}


	// 	

	return True;	
//LOGIN_ERROR:

}

// --------------------------
Bool csvryuser::OnLogOut(int sock, ssyNormalPacket& packet)
{
	return True;	
}

// --------------------------
// from m_UserDatahash.
/*int csvryuser::OnUserDataLoad(int Id, cyiList* pList)
{
	sycUserServerInfo*p = (sycUserServerInfo*)yialloc(sizeof(sycUserServerInfo));
	
	if (!p)
		return -1;

	
}*/

