#include "glsSystem.h"
#include "GlassInterface.h"
cglsSystem::cglsSystem(void)
: m_keyCenter(0)
, m_cmdCenter(0)
, m_performanceLevel(0)
{
	m_listCommand	= new cyiList(True);
	m_listKey		= new cyiList(True);
	m_listSysKey	= new cyiList(True);
	memset(&m_ptResolution, 0, sizeof(m_ptResolution));
}

cglsSystem::~cglsSystem(void)
{
}

typedef struct {
	cGlassInterface* obj;
	int nSystemId;
	int subId;
	int childId;
} sSystemStruct;

// --------------------------------------
void cglsSystem::RegisterCommand(cGlassInterface* obj, int nSystemId, int id/*=0*/)
{
	sSystemStruct data = {obj, nSystemId, id};
	m_listCommand->addref(xTOHEAP(&data));//yitoheap(&data, sizeof(data)));
}

// --------------------------------------
void cglsSystem::RegisterKeyCommand(int code, int nSystemId)
{
	sSystemStruct data = {0, nSystemId, code, 0};
	m_listKey->addref(xTOHEAP(&data));//yitoheap(&data, sizeof(data)));
}

// --------------------------------------
void cglsSystem::RegisterSystemKeyCommand(int code, int nSystemId)
{
	sSystemStruct data = {0, nSystemId, code, 0};
	m_listSysKey->addref(xTOHEAP(&data));//yitoheap(&data, sizeof(data)));
}

// --------------------------------------
void cglsSystem::EnableCommand(int nSystemId, Bool bIsEnable)
{
	m_listCommand->lock();
	for (cyiNode* node=m_listCommand->GetNodeTopFrom(); node; node=node->np)
	{
		sSystemStruct* system = (sSystemStruct*)node->ref;

		if (system)
		{
			if (system->nSystemId==nSystemId)
			{
				if (system->obj)
				{
					system->obj->Enable(bIsEnable, system->subId);
				}
			}
		}
	}
	m_listCommand->unlock();
}

// --------------------------------------
int cglsSystem::GetCommandId(cGlassInterface* obj, int id)
{
	m_listCommand->lock();
	for (cyiNode* node=m_listCommand->GetNodeTopFrom(); node; node=node->np)
	{
		sSystemStruct* system = (sSystemStruct*)node->ref;

		if (system->obj==obj &&
				system->subId==id)
		{
			m_listCommand->unlock();
			return system->nSystemId;
		}
	}
	m_listCommand->unlock();
	return 0;
}


// --------------------------------------
int cglsSystem::GetKeyCommandId(int code)
{
	m_listKey->lock();
	for (cyiNode* node=m_listKey->GetNodeTopFrom(); node; node=node->np)
	{
		sSystemStruct* system = (sSystemStruct*)node->ref;

		if (system->subId==code)
		{
			m_listKey->unlock();
			return system->nSystemId;
		}
	}
	m_listKey->unlock();

	return 0;
}

// --------------------------------------
int cglsSystem::GetSysKeyCommandId(int code)
{
	m_listSysKey->lock();
	for (cyiNode* node=m_listSysKey->GetNodeTopFrom(); node; node=node->np)
	{
		sSystemStruct* system = (sSystemStruct*)node->ref;

		if (system->subId==code)
		{
			m_listSysKey->unlock();
			return system->nSystemId;
		}
	}
	m_listSysKey->unlock();

	return 0;
}

// --------------------------------------
void cglsSystem::RegisterKeyCenter(cglsKeyCenter* center)
{
	m_keyCenter=center;
}

// --------------------------------------
void cglsSystem::RegisterCommandCenter(cglsCommandCenter* center)
{
	m_cmdCenter=center;
}

// --------------------------------------
cglsKeyCenter* cglsSystem::GetKeyCenter()
{
	return m_keyCenter;
}

// --------------------------------------
cglsCommandCenter* cglsSystem::GetCommandCenter()
{
	return m_cmdCenter;
}

// --------------------------------------
const char* cglsSystem::GetSystemDir()
{
	return m_szSystemDir;
}

// --------------------------------------
const char* cglsSystem::GetDataDir()
{
	return m_szDataDir;
}

// --------------------------------------
void cglsSystem::GetSystemDir(const char* pszSystemDir)
{
	strcpy(m_szSystemDir, pszSystemDir);
}

// --------------------------------------
void cglsSystem::GetDataDir(const char* pszDataDir)
{
	strcpy(m_szDataDir, pszDataDir);

}

// --------------------------------------
int cglsSystem::GetPerformance()
{
	return m_performanceLevel;
}

// --------------------------------------
void cglsSystem::SetPerformance(int n)
{
	m_performanceLevel=n;
}

// --------------------------------------
void cglsSystem::SetDisplayResolution(int x, int y)
{
	m_ptResolution.x=x;
	m_ptResolution.y=y;
}


// --------------------------------------
void cglsSystem::GetDisplayResolution(int&x, int&y)
{
	x=m_ptResolution.x;
	y=m_ptResolution.y;
}

// --------------------------------------
// 0: square.
// 1: 16:9
// 2: 16:10
int cglsSystem::GetDisplayType()
{
	static float fSquare	= 4.0/3.0;
	static float fwade		= 16.0/9.0;
	static float fwadeII	= 16.0/10.0;

	float rate = (float)m_ptResolution.x/(float)m_ptResolution.y;

	if (rate==fSquare)
		return 0;
	else if (rate==fwade)
		return 1;
	else if (rate==fwadeII)
		return 2;

	return 1;
}
