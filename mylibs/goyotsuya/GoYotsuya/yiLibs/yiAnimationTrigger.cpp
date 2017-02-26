#include "yiAnimationTrigger.h"

#define MAX_ANIMATION_PATTERN 128

Bool	g_bFunctionInitialized=False;
int		g_pattern[eGAP_MAX_PATTERN][MAX_ANIMATION_PATTERN];



// --------------------------------------------------------------------
static void xFuctionIntialize()
{
	int x;

	for (x=0; x<MAX_ANIMATION_PATTERN; x++)
	{
		g_pattern[eGAP_LINEAR][x]=x;
		g_pattern[eGAP_FISO][x]=MAX_ANIMATION_PATTERN-(((x-MAX_ANIMATION_PATTERN)*(x-MAX_ANIMATION_PATTERN))>>7);///MAX_ANIMATION_PATTERN;
		g_pattern[eGAP_SIFO][x]=(x*x)>>7;

		if (x<100)
		{
			g_pattern[eGAP_POPO][x]=(MAX_ANIMATION_PATTERN+26)-(((x-(100))*(x-(100)))>>7);
		}
		else
		{
			g_pattern[eGAP_POPO][x]= 254-x;//(MAX_ANIMATION_PATTERN+26)-(x-100);
		}
	}

	for (x=0; x<100; x++)
	{
		g_pattern[eGAP_BLINK][x]=(int)((float)((float)MAX_ANIMATION_PATTERN/100.0)*(float)x);
	}

	for (x=100; x<MAX_ANIMATION_PATTERN; x++)
	{
		g_pattern[eGAP_BLINK][x]=(int)(((float)((float)MAX_ANIMATION_PATTERN/100.0)*(float)x)*-1.0 + (float)MAX_ANIMATION_PATTERN);
	}

	g_bFunctionInitialized=True;
}

// --------------------------------------------------------------------
float yiAnitGetFactorF(float max, float index, eGlsAnimationPattern ePattern)
{
	float x;
	if (g_bFunctionInitialized==False)
	{
		xFuctionIntialize();
	}

	x = (float)((int)(index/max)<<7);

	return (float)g_pattern[ePattern][(int)__INTEGER__(x)];
}

// --------------------------------------------------------------------
float yiAnitGetFactor(int max, int index, eGlsAnimationPattern ePattern)
{
	int x;

	if (max<=index)
		return 1.0;

	if (g_bFunctionInitialized==False)
	{
		xFuctionIntialize();
	}
	x = ((index<<7)/max);

	return (float)g_pattern[ePattern][x]/128.0;
}

#include "yistd.h"

typedef struct xsAnit {
		HWND hWnd;
		UINT msg;
		int  nAnimationTerm;
		int  nAnimationInterval;
		int  nStoppedTime;
		HANDLE hThread;
		unsigned int threadId;
		Bool bIsStopped;
		unsigned int nAnimationProgress;
		unsigned int nAnimationStartTime;
		void* param;
		HandleMutexx hMutex;
		eGlsAnimationPattern pattern;
		float fBeforeFactor[eGAP_MAX_PATTERN];
} sAnit;

// --------------------------------------------------------------------
HandleAnit yiAnitCreate()
{
	sAnit anit;

	memset(&anit, 0, sizeof(anit));
	anit.hMutex = yiMutexCreateEx(5);
	anit.bIsStopped=True;
	return (HandleAnit)yitoheap(&anit, sizeof(anit));
}

// --------------------------------------------------------------------
void yiAnitDestroy(HandleAnit hAnimation)
{
	sAnit* anit = (sAnit*)hAnimation;

	if (hAnimation)
	{
		yiMutexDestroyEx(anit->hMutex);

		if (anit->hThread)
			CloseHandle(anit->hThread);
		yifree(anit);
	}
}

// --------------------------------------------------------------------
static unsigned int __stdcall xOnAnitThread(void* pParam)
{
	sAnit* anit = (sAnit*)pParam;
	int cnt = anit->nAnimationTerm/anit->nAnimationInterval;
	int i;

	anit->nStoppedTime=anit->nAnimationTerm;
	for (i=0; i<cnt+1; i++)
	{
		if (anit->bIsStopped==True)
		{
			anit->nStoppedTime=i*anit->nAnimationInterval;
			PostMessage(anit->hWnd, anit->msg, (WPARAM)-1, (LPARAM)anit->param);
			_endthreadex(0);
			anit->bIsStopped=True;

			return 0;
		}
		anit->nAnimationProgress=((i<<16)|(cnt));
		PostMessage(anit->hWnd, anit->msg, i, (LPARAM)anit->param);
		Sleep(anit->nAnimationInterval);
	}

	PostMessage(anit->hWnd, anit->msg, (WPARAM)-2, (LPARAM)anit->param);
	//if (anit->bIsStopped==False)
	//	anit->nAnimationProgress=0;

	//CloseHandle(anit->hThread);
	_endthreadex(0);
	anit->bIsStopped=True;



	return 0;
}

// --------------------------------------------------------------------
Bool yiAnitStart(HandleAnit hAnimation, HWND hNotifyWnd, UINT msg, 
					int nAnimationTerm, int nAnimationInterval, eGlsAnimationPattern pattern, void* pUserParam)
{
	sAnit* anit = (sAnit*)hAnimation;
	anit->nAnimationInterval=nAnimationInterval;
	anit->nAnimationTerm=nAnimationTerm;
	anit->param=pUserParam;
	anit->hWnd=hNotifyWnd;
	anit->msg=msg;
	anit->pattern=pattern;
	anit->bIsStopped=False;
	anit->nAnimationStartTime = cyiTime::CurrentmSec();

	if (nAnimationInterval==0)
	{
		PostMessage(anit->hWnd, anit->msg, (WPARAM)-1, (LPARAM)anit->param);
		return True;
	}

	if (anit->hThread)
		CloseHandle(anit->hThread);
	
	anit->nAnimationProgress=anit->nAnimationTerm/anit->nAnimationInterval;

	anit->hThread = (HANDLE)_beginthreadex(0, 0, xOnAnitThread, (void*)hAnimation, 0, &anit->threadId);

	if (anit->hThread==INVALID_HANDLE_VALUE ||
			anit->hThread==0)
	{
		return False;
	}

	return True;
}

// --------------------------------------------------------------------
// flag = 0 : before
// flag = 1 : end
// flag = 2 : 
int yiAnitStop(HandleAnit hAnimation, Bool bIsWait)
{
	sAnit* anit = (sAnit*)hAnimation;

	if (anit==0)
		return 0;

	anit->bIsStopped=True;

	if (bIsWait==True)
	{
		WaitForSingleObject(anit->hThread, INFINITE);
	}

//	ret = yiAnitGetDiffSec(hAnimation);
	
	//CloseHandle(anit->hThread);
	//anit->hThread=0;
	anit->param=0;
	anit->nAnimationStartTime=0;

	return anit->nStoppedTime;
}

// --------------------------------------------------------------------
void yiAnitClear(HandleAnit hAnimation)
{
	sAnit* anit = (sAnit*)hAnimation;
	anit->nAnimationProgress=0;
}

// --------------------------------------------------------------------
Bool yiAnitIsCleared(HandleAnit hAnimation, float* fAbortFactor)
{
	sAnit* anit = (sAnit*)hAnimation;

	if (anit->nAnimationProgress==0)
	{
		if (fAbortFactor)
			*fAbortFactor=0;
	}
	else 
	{
		if (fAbortFactor)
			*fAbortFactor = (float)((float)(anit->nAnimationProgress>>16)/(float)(anit->nAnimationProgress&0xFFFF));
	}

	return anit->nAnimationProgress==0?True:False;
}

void yiAnitWait(HandleAnit hAnimation)
{
	sAnit* anit = (sAnit*)hAnimation;
	WaitForSingleObject(anit->hThread, INFINITE);
	//CloseHandle(anit->hThread);
	//anit->hThread=0;
}

// --------------------------------------------------------------------
int yiAnitGetDiffSec(HandleAnit hAnimation)
{
	sAnit* anit = (sAnit*)hAnimation;

	unsigned long t = cyiTime::CurrentmSec();
	return cyiTime::GetSecDiff(anit->nAnimationStartTime, t);
}

// --------------------------------------------------------------------
void* yiAnitGetParam(HandleAnit hAnimation)
{
	sAnit* anit = (sAnit*)hAnimation;
	return anit->param;
}

// --------------------------------------------------------------------
unsigned int yiAnitGetId(HandleAnit hAnimation)
{
	sAnit* anit = (sAnit*)hAnimation;
	return anit->threadId;
}

const float g_fMinDelta = 0.005;

// --------------------------------------------------------------------
float yiAnitGetFactorEx(HandleAnit hAnimation, int i, eGlsAnimationPattern pattern)
{
	sAnit* anit = (sAnit*)hAnimation;
	int diff;
	int cnt = anit->nAnimationTerm/anit->nAnimationInterval;
	float fBeforeFactor = anit->fBeforeFactor[pattern];//fBeforeFactor[eGAP_MAX_PATTERN]={0.0, };

	if (i==0)
	{
		fBeforeFactor=0.0;
	}

	if (i>=(cnt))
	{
		return -1.0;
	}
	
	diff = cyiTime::GetSecDiff(anit->nAnimationStartTime, cyiTime::CurrentmSec());

	//xTRACE("yiAniGetFactor.... %u, %u, (%d) / %u", anit->nAnimationStartTime, diff, i, anit->nAnimationTerm);
	if (diff>anit->nAnimationTerm)
	{
		return -2.0;		
	}
	float f= yiAnitGetFactor(anit->nAnimationTerm, diff, pattern);

	if (i!=0)
	{
		if ((f-fBeforeFactor<g_fMinDelta))
		{
			return -4.0;
		}
	}
	fBeforeFactor=f;
	return f;
}

// --------------------------------------------------------------------
float yiAnitGetFactor(HandleAnit hAnimation, int i)
{

	sAnit* anit = (sAnit*)hAnimation;
	int diff;
	int cnt = anit->nAnimationTerm/anit->nAnimationInterval;

	if (i>=(cnt))
	{
		return -1.0;
	}

	float fBeforeFactor=anit->fBeforeFactor[anit->pattern];

	if (i==0)
		fBeforeFactor=0.0;
	

	diff = cyiTime::GetSecDiff(anit->nAnimationStartTime, cyiTime::CurrentmSec());

	//xTRACE("yiAniGetFactor.... %u, %u, (%d) / %u", anit->nAnimationStartTime, diff, i, anit->nAnimationTerm);
	if (diff>anit->nAnimationTerm)
	{
		return -2.0;		
	}
	float f= yiAnitGetFactor(anit->nAnimationTerm, diff, anit->pattern);

	if (i!=0)
	{
		if ((f-fBeforeFactor)<g_fMinDelta)
		{
			return -4.0;
		}
	}

	fBeforeFactor=f;

	return f;
}

// --------------------------------------------------------------------
Bool yiAnitIsAlived(HandleAnit hAnimation)
{
	sAnit* anit = (sAnit*)hAnimation;

	//if (WaitForSingleObject(anit->hThread, 0)==WAIT_TIMEOUT)
	//{
	//	return True;
	//}

	return anit->bIsStopped ? False:True;
}
