#ifndef __svrydefs_load_h__
#define __svrydefs_load_h__

// ----------------------------
typedef struct xsAtypeData 
{
	unsigned int id;
	char name[16];
	char password[8];
	unsigned int area_code;
	unsigned int birth_day;
	unsigned int flag32; /*8byte:job, 8byte:level, remain is reserved.*/
	unsigned int point;
	unsigned int member_license;
	unsigned int software_license;
	unsigned int startday;
	unsigned int lastlogin;
	unsigned int lastupdate;
	unsigned int lastlogout;
	char email_addr[64];
} sATypeData;

// ----------------------------
typedef struct xBTypeData
{
	unsigned int cnt;
	unsigned int ids[1];
} sBTypeData;

#endif

