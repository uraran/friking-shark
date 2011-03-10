// GameGUI.cpp : Defines the entry point for the DLL application.
//

#include "./stdafx.h"
#include "GameGUI.h"
#include "GameGUILib.h"
#include "GameGUILabel.h"
#include "GameGUIButton.h"
#include "GameGUIGroup.h"
#include "GameGUIEdit.h"
#include "GameGUIList.h"
#include "GameGUIManager.h"
#include "GameGUIMessageDialog.h"
#include "GameGUIConfirmDialog.h"
#include "GameGUIColorDialog.h"
#include "GameGUIFileDialog.h"
#include "GameGUIInputDialog.h"

BEGIN_SYSTEM_MODULE()
	SYSTEM_MODULE_CLASS_FACTORY_ENTRY(CGameGUIManager,"CGameGUIManager")
	SYSTEM_MODULE_CLASS_FACTORY_ENTRY(CGameGUIButton,"CGameGUIButton")
	SYSTEM_MODULE_CLASS_FACTORY_ENTRY(CGameGUILabel,"CGameGUILabel")
	SYSTEM_MODULE_CLASS_FACTORY_ENTRY(CGameGUIGroup,"CGameGUIGroup")
	SYSTEM_MODULE_CLASS_FACTORY_ENTRY(CGameGUIEdit,"CGameGUIEdit")
	SYSTEM_MODULE_CLASS_FACTORY_ENTRY(CGameGUIList,"CGameGUIList")
	SYSTEM_MODULE_CLASS_FACTORY_ENTRY(CGameGUIMessageDialog,"CGameGUIMessageDialog")
	SYSTEM_MODULE_CLASS_FACTORY_ENTRY(CGameGUIConfirmDialog,"CGameGUIConfirmDialog")
	SYSTEM_MODULE_CLASS_FACTORY_ENTRY(CGameGUIColorDialog,"CGameGUIColorDialog")
	SYSTEM_MODULE_CLASS_FACTORY_ENTRY(CGameGUIFileDialog,"CGameGUIFileDialog")
	SYSTEM_MODULE_CLASS_FACTORY_ENTRY(CGameGUIInputDialog,"CGameGUIInputDialog")
END_SYSTEM_MODULE()

