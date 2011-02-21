#include "./stdafx.h"
#include "GameMainWindow.h"

CGameMainWindow::CGameMainWindow(IGameGUIManager *piGUIManager)
{
	m_piGUIManager=piGUIManager;
}

CGameMainWindow::~CGameMainWindow(void)
{
}

bool CGameMainWindow::InitWindow(IGameWindow *piParent,bool bPopup){return true;}
void CGameMainWindow::DestroyWindow()
{
	while(m_vChildren.size())
	{
		IGameWindow *piWindow=m_vChildren[0];
		ADD(piWindow);
		piWindow->DestroyWindow(); // DestroyWindow elimina la referencia de m_vChildren al llamar a RemoveChild
		REL(piWindow);
	}
}

IGameGUIManager *CGameMainWindow::GetGUIManager()
{
	ADD(m_piGUIManager);
	return m_piGUIManager;
}

bool CGameMainWindow::IsPopup(){return false;}

void CGameMainWindow::Show(bool bShow){}
bool CGameMainWindow::IsVisible(){return true;}
void CGameMainWindow::Activate(bool bActive){}
bool CGameMainWindow::IsActive(){return true;}

void CGameMainWindow::SetWindowName(std::string sName){}

std::string CGameMainWindow::GetWindowName()
{
	return "";
}

void CGameMainWindow::GetRect(SGameRect *pRect)
{
	pRect->x=0.0;
	pRect->y=0.0;
	pRect->w=1.0;
	pRect->h=1.0;
}

void CGameMainWindow::SetRect(SGameRect *pRect)
{
}

void CGameMainWindow::GetRealRect(SGameRect *pRect)
{
	SGameSize size;
	m_piGUIManager->GetWindowSize(&size);
	pRect->x=0;
	pRect->y=0;
	pRect->w=size.w;
	pRect->h=size.h;
}

void CGameMainWindow::UpdateRealRect()
{
	for(unsigned x=0;x<m_vChildren.size();x++)
	{
		m_vChildren[x]->UpdateRealRect();
	}
}

void CGameMainWindow::SetReferenceSystem(eGameGUIReferenceSystem eRefSystem){}
eGameGUIReferenceSystem CGameMainWindow::GetReferenceSystem(){return eGameGUIReferenceSystem_Relative;}

void CGameMainWindow::SetFont(IGenericFont *piFont,double dSize)
{
}

void CGameMainWindow::GetFont(IGenericFont **ppiFont,double *pdSize)
{
	(*ppiFont)=NULL;
	(*pdSize)=0;
}

IGameWindow *CGameMainWindow::GetParent(){return NULL;}

void CGameMainWindow::GetChildren(std::vector<IGameWindow *> *pvChildren)
{
	std::vector<IGameWindow*>::iterator i;

	for(i=m_vChildren.begin();i!=m_vChildren.end();i++)
	{
		IGameWindow *piWindow=*i;
		ADD(piWindow);
		pvChildren->push_back(piWindow);
	}
}

void CGameMainWindow::AddChild(IGameWindow *piWindow)
{
	ADD(piWindow);
	m_vChildren.push_back(piWindow);
}

void CGameMainWindow::RemoveChild(IGameWindow *piWindow)
{
	std::vector<IGameWindow*>::iterator i;

	for(i=m_vChildren.begin();i!=m_vChildren.end();i++)
	{
		IGameWindow *piTempWindow=*i;
		if(piTempWindow==piWindow)
		{
			m_vChildren.erase(i);
			REL(piTempWindow);
			break;
		}
	}
}

IGameWindow *CGameMainWindow::FindChild(std::string sName)
{
	std::vector<IGameWindow*>::iterator i;

	for(i=m_vChildren.begin();i!=m_vChildren.end();i++)
	{
		IGameWindow *piWindow=*i;
		if(piWindow->GetWindowName()==sName)
		{
			ADD(piWindow);
			return piWindow;
		}
	}
	for(i=m_vChildren.begin();i!=m_vChildren.end();i++)
	{
		IGameWindow *piWindow=(*i)->FindChild(sName);
		if(piWindow)
		{
			return piWindow;
		}
	}
	return NULL;
}

void CGameMainWindow::BringToFront()
{
}

void CGameMainWindow::BringChildToFront(IGameWindow *piWindow)
{
	std::vector<IGameWindow*>::iterator i;

	for(i=m_vChildren.begin();i!=m_vChildren.end();i++)
	{
		if(*i==piWindow)
		{
			m_vChildren.erase(i);
			m_vChildren.push_back(piWindow);
			break;
		}
	}
}
void CGameMainWindow::EnumerateChildren(IGameWindowEnumerationCallback *piCallback)
{
	bool bStopEnumerating=false;
	std::vector<IGameWindow*>::iterator i;

	for(i=m_vChildren.begin();i!=m_vChildren.end() && !bStopEnumerating;i++)
	{
		piCallback->ProcessEnumeratedWindow(*i,&bStopEnumerating);
	}
}

void CGameMainWindow::OnCharacter( int nKey,bool *pbProcessed )
{

}

void CGameMainWindow::OnKeyDown(int nKey,bool *pbProcessed)
{
	if(nKey==VK_ESCAPE)
	{
		m_piGUIManager->ExitGUILoop();
	}
}
void CGameMainWindow::OnKeyUp(int nKey,bool *pbProcessed){}

void CGameMainWindow::OnMouseDown(int nButton,double x,double y){}
void CGameMainWindow::OnMouseUp(int nButton,double x,double y){}
void CGameMainWindow::OnMouseMove(double x,double y){}
void CGameMainWindow::OnDrawMouseCursor(SGamePos position,IGenericRender *piRender,bool *pbDrawed){}
void CGameMainWindow::OnDrawBackground(IGenericRender *piRender){}
void CGameMainWindow::OnDraw(IGenericRender *piRender){}

void CGameMainWindow::OnSetFocus(){}
void CGameMainWindow::OnKillFocus(IGameWindow *piFocusedWindow){}
void CGameMainWindow::OnReleaseMouseCapture(){}

IGenericTexture *CGameMainWindow::GetBackgroundTexture()
{
	return NULL;
}
void CGameMainWindow::SetBackgroundTexture(IGenericTexture *piTexture)
{}

IGenericModel *CGameMainWindow::GetBackgroundModel()
{
	return NULL;
}
void CGameMainWindow::SetBackgroundModel(IGenericModel *piModel)
{}

void	CGameMainWindow::GetBackgroundColor(CVector *pvColor,double *pdAlpha)
{
	*pvColor=CVector(0,0,0);
	*pdAlpha=0;
}

void	CGameMainWindow::SetBackgroundColor(CVector vColor,double dAlpha)
{
}
