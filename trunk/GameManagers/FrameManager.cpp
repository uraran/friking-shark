#include "./stdafx.h"
#include "FrameManager.h"

#ifndef WIN32
#include <sys/time.h>
#endif


CFrameManager::CFrameManager()
{
	m_dwCurrentRealTime=0;
	m_dwCurrentTime=0;
	m_dTimeFraction=0;
	m_dRealTimeFraction=0;
	m_dwTimeBase=0;
	m_dwLastTime=0;
	m_dwLastRealTime=0;
	m_bPaused=false;
	m_dwPauseStartTime=0;

	m_bTogglePauseOnNextFrame=false;
	m_bContinueOnNextFrame=false;
	m_bSetPauseOnNextFrame=false;

	m_dCurrentFps=0;
	m_nFPSFrames=0;
	memset(&m_dwFPSFrameTimes,0,sizeof(m_dwFPSFrameTimes));

#ifdef WIN32
	QueryPerformanceFrequency(&m_ldPerformanceFrequency);
#endif 
}

DWORD  CFrameManager::GetCurrentRealTime(){return m_dwCurrentRealTime;}
DWORD  CFrameManager::GetCurrentTime(){return m_dwCurrentTime;}
double CFrameManager::GetTimeFraction(){return m_dTimeFraction;}
double CFrameManager::GetRealTimeFraction(){return m_dRealTimeFraction;}

void CFrameManager::Reset()
{
#ifdef WIN32
	LARGE_INTEGER ldNow={0};
	QueryPerformanceCounter(&ldNow);
	m_dwTimeBase=(DWORD)(ldNow.QuadPart*1000/m_ldPerformanceFrequency.QuadPart);
#else
	timeval tNow;
    gettimeofday(&tNow, NULL);
	m_dwTimeBase=((double)tNow.tv_sec)*1000.0+((double)tNow.tv_usec)/1000.0;
#endif
	m_dwLastTime=0;
	m_dwCurrentTime=0;
}

bool CFrameManager::IsPaused()
{
	return m_bPaused;
}

void CFrameManager::SetPauseOnNextFrame(bool bPause)
{
	m_bSetPauseOnNextFrame=bPause;
	m_bContinueOnNextFrame=!bPause;
}

void CFrameManager::TogglePauseOnNextFrame()
{
	m_bTogglePauseOnNextFrame=true;
}

void CFrameManager::ExecuteTogglePause()
{
	if(m_bPaused)
	{
		m_bPaused=false;
		m_dwTimeBase+=(m_dwCurrentRealTime-m_dwTimeBase)-m_dwPauseStartTime;
	}
	else
	{
		m_bPaused=true;
		m_dwPauseStartTime=m_dwCurrentTime;
	}
}

void CFrameManager::ProcessFrame()
{
	if(m_bTogglePauseOnNextFrame)
	{
		ExecuteTogglePause();
	}
	else if(m_bSetPauseOnNextFrame)
	{
		if(!m_bPaused){ExecuteTogglePause();}
	}
	else if(m_bContinueOnNextFrame)
	{
		if(m_bPaused){ExecuteTogglePause();}
	}
	m_bTogglePauseOnNextFrame=m_bSetPauseOnNextFrame=m_bContinueOnNextFrame=false;

#ifdef WIN32
	LARGE_INTEGER ldNow={0};
	QueryPerformanceCounter(&ldNow);
	m_dwCurrentRealTime=(DWORD)(ldNow.QuadPart*1000/m_ldPerformanceFrequency.QuadPart);
#else
	timeval tNow;
    gettimeofday(&tNow, NULL);
	m_dwCurrentRealTime=((double)tNow.tv_sec)*1000.0+((double)tNow.tv_usec)/1000.0;
#endif
	if(!m_bPaused)
	{
		m_dwCurrentTime=m_dwCurrentRealTime-m_dwTimeBase;
		// Limitacion del tiempo maximo de procesado de framerate a 50 ms (20fps).
		// Con esto se evitan saltos al depurar y cuando se produce un tiron de disco.
		if(m_dwCurrentTime>(m_dwLastTime+50))
		{
			m_dwTimeBase+=(m_dwCurrentTime-(m_dwLastTime+50));
			m_dwCurrentTime=m_dwLastTime+50;
		}
	}
	m_dTimeFraction=((double)(m_dwCurrentTime-m_dwLastTime))/1000.0;
	m_dRealTimeFraction=((double)(m_dwCurrentRealTime-m_dwLastRealTime))/1000.0;
	m_dwLastTime=m_dwCurrentTime;
	m_dwLastRealTime=m_dwCurrentRealTime;

	ComputeFps();
}

void CFrameManager::ComputeFps()
{
	unsigned nFramesToRemove=0;
	for(unsigned _fps=0;_fps<m_nFPSFrames;_fps++)
	{
		if(m_dwCurrentRealTime<1000 || m_dwFPSFrameTimes[_fps]>m_dwCurrentRealTime-1000)
		{
			break;
		}
		else
		{
			nFramesToRemove++;
		}
	}
	if(nFramesToRemove)
	{
		memmove(m_dwFPSFrameTimes,m_dwFPSFrameTimes+nFramesToRemove,sizeof(float)*(m_nFPSFrames-nFramesToRemove));
		m_nFPSFrames-=nFramesToRemove;
	}
	m_dwFPSFrameTimes[m_nFPSFrames]=m_dwCurrentRealTime;
	m_nFPSFrames++;
	m_dCurrentFps=m_nFPSFrames;
}

double CFrameManager::GetCurrentFps()
{
	return m_dCurrentFps;
}