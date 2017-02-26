#include "yctrls.h"
#include "GoYotsuyaResource.h"
#include "yiResource.h"
#include "cyctrlResource.h"
#include "yiGlassInterfaceDraw.h"

GdiplusStartupInput __g_gdiSI__;
ULONG_PTR           __g_gdiToken__;


#include "yiEnv.h"

#include "glsDrawObject.h"
#include "glsResourceBitmap.h"

extern cglsResourceObject* g_resourceObject;
Bool g_bInitialized=False;

// --------------------------------
Bool yictrl_initialize()
{
	if (g_bInitialized==True)
		return True;

	//memset(&__g_gdiSI__, 0, sizeof(__g_gdiSI__));
	if (GdiplusStartup(&__g_gdiToken__, &__g_gdiSI__, NULL)!=Status::Ok)
	{
		return False;
	}

	g_bInitialized=True;
#ifdef STATIC_COMPILE
	g_resourceObject = new cglsResourceBitmap();
#endif

	//char buffer[256];

	//cyiEnv::MakeEnv();
	//wsprintf(buffer, "%s\\images", cyiEnv::GetYotsuyaRootDir());
	//cyiResource::GetInstance(eRTX_IMAGE)->Load(buffer);

	//wsprintf(buffer, "%s\\images.cashed", cyiEnv::GetYotsuyaRootDir());
	//cyiResource::GetInstance(eRTX_IMAGE_CACHED)->Load(buffer);


	//cyiGlassInterfaceDraw::MakeShadowImage(eRBID_SHADOW_10, DIALOG_SHADOW_WIDTH, 5.0, 1.2);
	//cyiGlassInterfaceDraw::MakeShadowImage(eRBID_SHADOW_2, DIALOG_SHADOW_WIDTH, 1.0, 1.0);
	//cyiGlassInterfaceDraw::MakeShadowImage(eRBID_SHADOW_0, DIALOG_SHADOW_WIDTH, 0, 1.0);


	//Font* fontButton = new Font(L"Meiryo UI", 10);
	//Font* fontDialog = new Font(L"Meiryo UI", 12);
	//Font* fontDialogCaption = new Font(L"Meiryo UI", 10.0, FontStyleBold);


	//cyiResource::GetInstance(eRTX_TEMPORARY)->Register(eRTID_BUTTON_FONT , fontButton, 0);
	//cyiResource::GetInstance(eRTX_TEMPORARY)->Register(eRTID_DIALOG_FONT , fontDialog, 0);
	//cyiResource::GetInstance(eRTX_TEMPORARY)->Register(eRTID_DIALOG_CAPTION_FONT , fontDialogCaption, 0);
	//cyiResource::GetInstance(eRTX_TEMPORARY)->Register(eRTID_DEFAULT_FONT, fontDialogCaption, 0);

}

// --------------------------------
void yictrl_finalize()
{
	//delete cyiResource::GetInstance(eRTX_IMAGE);
	//delete cyiResource::GetInstance(eRTX_IMAGE_CACHED);
	//delete cyiResource::GetInstance(eRTX_TEMPORARY);

	GdiplusShutdown(__g_gdiToken__);
}