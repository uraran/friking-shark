#include "./stdafx.h"
#include "GameRunTimeLib.h"
#include "SystemLoaderHelper.h"

CSystemLoaderHelper::CSystemLoaderHelper(void)
{
}

CSystemLoaderHelper::~CSystemLoaderHelper(void)
{
}

ISystem *CSystemLoaderHelper::LoadSystem(std::string sFile,std::string sSystemName)
{
	CConfigFile configFile;
	if(configFile.Open(sFile))
	{
		ISystem *piSystem=LoadSystem(configFile.GetRoot(),sSystemName);
		if(piSystem==NULL){RTTRACE("CSystemLoaderHelper::LoadSystem -> Failed to load system %s from file %s",sSystemName.c_str(),sFile.c_str());}
		return piSystem;
	}
	else
	{
		RTTRACE("CSystemLoaderHelper::LoadSystem -> Failed to open config file %s to load system %s",sFile.c_str(),sSystemName.c_str());
	}
	return NULL;
}

ISystem *CSystemLoaderHelper::LoadSystem(ISystemPersistencyNode *piParent,std::string sSystemName)
{
    bool bResult=true;
    ISystemPersistencyNode  *piNode=piParent->GetNode(sSystemName);
    ISystem                 *piSystem=NULL;

	if(piNode==NULL){return NULL;}

    ISystemManager *piSystemManager=GetSystemManager();
    piSystem=piSystemManager->CreateSystem(sSystemName);
    if(piSystem!=NULL)
		{
			if(m_Modules.PersistencyLoad(piNode))
			{
				unsigned x=0;
				for(x=0;x<m_Modules.m_dModules.size();x++)
				{
					ISystemModule *piModule=NULL;
					if(!piSystem->LoadModule(m_Modules.m_dModules[x].sPath,&piModule))
					{
						bResult=false;
						RTTRACE("CSystemLoaderHelper::LoadSystem -> Failed to load Module %s in System %s",m_Modules.m_dModules[x].sPath.c_str(),sSystemName.c_str());
					}
					REL(piModule);
				}
					// Carga de objetos.
				PersistencyLoad(piNode);
				for(x=0;x<m_dObjects.size();x++)
				{
					m_dObjects[x].Detach();
				}
				m_dObjects.clear();
			}
			else
			{
					piSystemManager->UnregisterSystem(piSystem);
			}
		}
    REL(piSystemManager);
    if(!bResult)
	{
		if(piSystem){piSystem->Destroy();}
		REL(piSystem);
	}
    return piSystem;
}


bool CSystemSaverHelper::SaveSystemNamedObjects(std::string sFile,std::string sSystemName)
{
	CConfigFile configFile;
	if(!configFile.Open(sFile))
	{
		RTTRACE("CSystemSaverHelper::SaveSystemNamedObjects -> Failed to open config file %s to save system %s objects",sSystemName.c_str());
	}	
	
	ISystemPersistencyNode  *piNode=configFile.GetRoot()->AddNode(sSystemName);
	bool bOk=(piNode!=NULL);
	if(bOk){bOk=PersistencyLoad(piNode);}
	if(bOk){bOk=SaveSystemNamedObjects(piNode,sSystemName);}
	if(bOk){bOk=configFile.Save(sFile);}	
	return bOk;
}

bool CSystemSaverHelper::SaveSystemNamedObjects(ISystemPersistencyNode *piNode,std::string sSystemName)
{
	std::vector<ISystemObject *> vObjects;
	GetSystemObjects(sSystemName,&vObjects);
	for(unsigned long x=0;x<vObjects.size();x++)
	{
		ISystemObject *piObject=vObjects[x];
		CSystemObjectWrapper wrapper;
		// Aqui se usa create para que el wrapper serialice el objeto
		if(wrapper.Create(sSystemName,piObject->GetClass(),piObject->GetName()))
		{
			m_dObjects.push_back(wrapper);
			
		}
	}
	for(unsigned long x=0;x<vObjects.size();x++){ISystemObject *piObject=vObjects[x];REL(piObject);}
	// Carga de objetos.
	return PersistencySave(piNode);
}

CSystemSaverHelper::CSystemSaverHelper(){}
CSystemSaverHelper::~CSystemSaverHelper(){}

