#pragma once

#define WM_GO_HEADER_CHANGED (WM_USER+258)

typedef struct {
	char		szSoftwareName[32];
	unsigned int day;
	unsigned int time;
	unsigned int flag32; // 31: no license, 30: limited free
	unsigned int license[4];
	unsigned int installedId;
} sSoftwareLisenseCode;

// Joseki Free license code.
// 

enum eGobanAppMode {
	eGAM_NORMAL=0,
	eGAM_JOSEKI_EDITOR,
	eGAM_JOSEKI_PLAYER,
	eGAM_GO_MAIL
};