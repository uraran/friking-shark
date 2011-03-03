#pragma once

DECLARE_CUSTOM_WRAPPER1(CRenderWrapper,IGenericRender,m_piRender)
DECLARE_CUSTOM_WRAPPER1(CViewportWrapper,IGenericViewport,m_piViewport)

class CGameGUIManager :	virtual public CSystemObjectBase,virtual public IGameGUIManager,virtual public IGenericViewportCallBack
{
	IGameWindow *m_piMainWindow;
	IGameWindow *m_piFocusedWindow;
	IGameWindow *m_piMouseCaptureWindow;

	bool		m_bShowMouseCursor;
	bool		m_bScreenSettingsChanged;
	SVideoMode  m_sOldVideoMode;
	SGameScreenProperties m_sScreenProperties;

	CRenderWrapper m_Render;
	CViewportWrapper m_Viewport;

	std::vector<IGameWindow*> m_vPopups;

	void RenderWindow(IGenericRender *piRender,IGameWindow *piWindow,SGameRect rParentClipRect);
	void ProcessMouseActivation(IGameWindow *piWindow);

	IGameWindow *GetWindowFromPos(SGamePos *pPosition,bool bOnlyActive);
	IGameWindow *GetWindowFromPos(IGameWindow *piWindow,SGamePos *pPosition,bool bOnlyActive);

	// IGenericViewportCallBack

	void OnRender();

	void OnLButtonDown(unsigned x,unsigned y);
	void OnLButtonDoubleClick(unsigned x,unsigned y);
	void OnLButtonUp(unsigned x,unsigned y);
	void OnRButtonDown(unsigned x,unsigned y);
	void OnRButtonUp(unsigned x,unsigned y);
	void OnRButtonDoubleClick(unsigned x,unsigned y);
	void OnMouseMove(unsigned x,unsigned y);
	void OnMouseWheelUp(unsigned x,unsigned y);
	void OnMouseWheelDown(unsigned x,unsigned y);
	
	void OnCharacter(unsigned short wCharacter);
	void OnKeyDown(unsigned short wKey);
	void OnKeyUp(unsigned short wKey);
	
	void OnSize(unsigned cx,unsigned cy);
	void OnMove();
	
	void UpdateScreenPlacement();

	bool Unserialize(ISystemPersistencyNode *piNode);

public:

	BEGIN_PROP_MAP(CGameGUIManager)
		PROP_FLAGS(m_sScreenProperties,"Screen",MRPF_NORMAL|MRPF_OPTIONAL);
	END_PROP_MAP()

	// IGameGUIManager overloaded.

	bool Init(std::string sClass,std::string sName,ISystem *piSystem);
	void Destroy();

	void EnterGUILoop(); 
	void ExitGUILoop(); 

	void		GetWindowSize(SGameSize *pSize);
	void		GetMousePosition(IGameWindow *piWindow,SGamePos *pPos);
	void		SetMousePosition(IGameWindow *piWindow,SGamePos &pos);

	bool		IsKeyDown(int nKey);
	bool		IsMouseDown(int nMouseButton);

	void		SetFocus(IGameWindow *piWindow);
	bool		HasFocus(IGameWindow *piWindow);

	void		SetMouseCapture(IGameWindow *piWindow);
	void		ReleaseMouseCapture();
	bool		HasMouseCapture(IGameWindow *piWindow);

	void 		ShowMouseCursor(bool bShow);
	bool 		IsMouseCursorVisible();

	void		RedrawAll();

	IGameWindow *GetMainWindow();

	void		GetScreenProperties(SGameScreenProperties *pProperties);
	void    	SetScreenProperties(SGameScreenProperties *pProperties);

	// Popup management.

	void		 AddPopup(IGameWindow *);
	void		 RemovePopup(IGameWindow *);
	void		 EnumeratePopups(IGameWindowEnumerationCallback *piCallback);
	void		 GetPopups(std::vector<IGameWindow *> *pvPopups);

	bool 		 DetectDrag(double dx,double dy);
	
	CGameGUIManager(void);
	~CGameGUIManager(void);
};