#pragma once

#include "yistd.h"

class cyiSetup {
	cyiSetup(void);
	~cyiSetup(void);
};

class xEXPORT cyiEnv
{
public:
	cyiEnv(void);
	~cyiEnv(void);

	// -------------------------
	static void MakeEnv();

	// -------------------------
	static const char* GetYotsuyaRootDir();

	// -------------------------
	static const char* GetYotsuyaUserDir();

	// -------------------------
	static const char* GetYotsuyaUserDir(unsigned int id, const char* pszUserName);

	// -------------------------
	static const char* GetYotsuyaCacheDir();

	// -------------------------
	static cyiSetup* GetUserSetup(unsigned int id, const char* pszUserName);

};
