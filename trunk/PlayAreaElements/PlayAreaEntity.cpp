#include "./stdafx.h"
#include "GameGraphics.h"
#include "PlayAreaElementBase.h"
#include "PlayAreaEntity.h"

CPlayAreaEntity::CPlayAreaEntity()
{
	m_dRadius=0;
    m_nDelay=0;
	m_nInterval=0;
	m_nEntityCount=1;
	m_nActivationTime=0;
	m_nLastEntityTime=0;
	m_nCreatedEntities=0;
}

CPlayAreaEntity::~CPlayAreaEntity()
{
    m_EntityType.Detach();
}

bool CPlayAreaEntity::ProcessFrame(CVector vPlayPosition,SPlayAreaInfo *pAreaInfo,unsigned int dwCurrentTime,double dInterval)
{
	if(m_EntityType.m_piEntityType && m_dRadius==0){m_dRadius=m_EntityType.m_piEntityType->DesignGetRadius();}
		
	bool bCurrentlyInPlayArea=Util_IsInPlayArea(m_vPosition,pAreaInfo);
	bCurrentlyInPlayArea=bCurrentlyInPlayArea||Util_IsInPlayArea(m_vPosition-CVector(m_dRadius,0,0),pAreaInfo);
	bCurrentlyInPlayArea=bCurrentlyInPlayArea||Util_IsInPlayArea(m_vPosition+CVector(m_dRadius,0,0),pAreaInfo);

	if(m_bActive)
	{
        if(m_EntityType.m_piEntityType!=NULL && 
			((m_nCreatedEntities==0 && dwCurrentTime>=m_nActivationTime+m_nDelay) 
			|| (m_nCreatedEntities<m_nEntityCount && dwCurrentTime>=(m_nLastEntityTime+m_nInterval))))
        {
			IEntity *piEntity=m_EntityType.m_piEntityType->CreateInstance(NULL,dwCurrentTime);
			piEntity->GetPhysicInfo()->vPosition=m_vPosition;
			piEntity->GetPhysicInfo()->vAngles=m_vAngles;
			if(m_Route.GetPointCount()){piEntity->SetRoute(&m_Route);}
			SUBSCRIBE_TO(piEntity,IEntityEvents);
			m_sEntities.insert(piEntity);
			m_nCreatedEntities++;
			m_nLastEntityTime=dwCurrentTime;
		}

		if(m_Route.GetPointCount()==0)
		{
			set<IEntity*>::iterator i;
			for(i=m_sEntities.begin();i!=m_sEntities.end();)
			{
				IEntity *piEntity=*i;
				bool bEntityCurrentlyInPlayArea=Util_IsInPlayArea(piEntity->GetPhysicInfo()->vPosition,pAreaInfo);
				bEntityCurrentlyInPlayArea=bEntityCurrentlyInPlayArea||Util_IsInPlayArea(piEntity->GetPhysicInfo()->vPosition-CVector(m_dRadius,0,0),pAreaInfo);
				bEntityCurrentlyInPlayArea=bEntityCurrentlyInPlayArea||Util_IsInPlayArea(piEntity->GetPhysicInfo()->vPosition+CVector(m_dRadius,0,0),pAreaInfo);
				if(!bEntityCurrentlyInPlayArea)
				{
					m_sEntities.erase(i++);
					UNSUBSCRIBE_FROM_CAST(piEntity,IEntityEvents);
					piEntity->Remove();
					piEntity=NULL;
				}
				else{i++;}
			}
		}
		if(m_sEntities.size()==0 && !bCurrentlyInPlayArea)
		{
			Deactivate();
		}
	}
	else
	{
		if(bCurrentlyInPlayArea)
		{
			Activate(dwCurrentTime);
		}
	}
	return m_bActive;
}

bool CPlayAreaEntity::Init(std::string sClass,std::string sName,ISystem *piSystem)
{
    bool bOk=CPlayAreaElementBase::Init(sClass,sName,piSystem);
    return bOk;
}

void CPlayAreaEntity::Destroy()
{
	Deactivate();
	m_dRadius=0;
	m_EntityType.Detach();
	CPlayAreaElementBase::Destroy();
}

void CPlayAreaEntity::Activate(unsigned int dwCurrentTime)
{
    CPlayAreaElementBase::Activate(dwCurrentTime);
}

void CPlayAreaEntity::Deactivate()
{
	set<IEntity*>::iterator i;
	for(i=m_sEntities.begin();i!=m_sEntities.end();i++)
    {
		IEntity *piEntity=*i;
        UNSUBSCRIBE_FROM_CAST(piEntity,IEntityEvents);
        piEntity->Remove();
        piEntity=NULL;
    }
	m_sEntities.clear();
	m_nCreatedEntities=0;
	m_nLastEntityTime=0;
    CPlayAreaElementBase::Deactivate();
}

void CPlayAreaEntity::OnKilled(IEntity *piEntity)
{
	UNSUBSCRIBE_FROM_CAST(piEntity,IEntityEvents);
	m_sEntities.erase(piEntity);
	if(m_sEntities.size()==0 && m_nCreatedEntities==m_nEntityCount){Deactivate();}
}

void CPlayAreaEntity::OnRemoved(IEntity *piEntity)
{
	UNSUBSCRIBE_FROM_CAST(piEntity,IEntityEvents);
	m_sEntities.erase(piEntity);
	if(m_sEntities.size()==0 && m_nCreatedEntities==m_nEntityCount){Deactivate();}
}

void CPlayAreaEntity::SetPosition(const CVector &vPosition)
{
	m_vPosition=vPosition;
	if(m_bActive && m_Route.GetPointCount()==0)
	{
		set<IEntity*>::iterator i;
		for(i=m_sEntities.begin();i!=m_sEntities.end();i++)
	    {
			IEntity *piEntity=*i;
			piEntity->GetPhysicInfo()->vPosition=m_vPosition;
		}
	}
}

void CPlayAreaEntity::SetAngles(const CVector &vAngles)
{
	m_vAngles=vAngles;
	if(m_bActive && m_Route.GetPointCount()==0)
	{
		set<IEntity*>::iterator i;
		for(i=m_sEntities.begin();i!=m_sEntities.end();i++)
	    {
			IEntity *piEntity=*i;
			piEntity->GetPhysicInfo()->vAngles=m_vAngles;
		}
	}
}
void CPlayAreaEntity::SetEntityType(IEntityType *piEntityType)
{
	m_EntityType.Attach(piEntityType);
	if(m_EntityType.m_piEntityType){m_dRadius=m_EntityType.m_piEntityType->DesignGetRadius();}
}
void CPlayAreaEntity::SetCount(unsigned int nCount){m_nEntityCount=nCount;}
void CPlayAreaEntity::SetDelay(unsigned int nDelay){m_nDelay=nDelay;}
void CPlayAreaEntity::SetInterval(unsigned int nInterval){m_nInterval=nInterval;}

CVector CPlayAreaEntity::GetPosition(){return m_vPosition;}
CVector CPlayAreaEntity::GetAngles(){return m_vAngles;}
void	CPlayAreaEntity::GetEntityType(IEntityType **ppiEntityType){if(ppiEntityType){*ppiEntityType=ADD(m_EntityType.m_piEntityType);}	}

unsigned int CPlayAreaEntity::GetCount(){return m_nEntityCount;}
unsigned int CPlayAreaEntity::GetDelay(){return m_nDelay;}
unsigned int CPlayAreaEntity::GetInterval(){return m_nInterval;}

void CPlayAreaEntity::DesignRender( IGenericRender *piRender,bool bSelected )
{
	if(m_EntityType.m_piEntityType==NULL){return;}
	m_EntityType.m_piEntityType->DesignRender(piRender,m_vPosition,m_vAngles,bSelected);
}

CTraceInfo CPlayAreaEntity::DesignGetTrace( const CVector &p1,const CVector &p2 )
{
	if(m_EntityType.m_piEntityType==NULL)
	{
		CTraceInfo info;
		info.m_dTraceFraction=1.0;
		info.m_vTracePos=p2;
		return info;
	}
	else
	{
		return m_EntityType.m_piEntityType->DesignGetTrace(m_vPosition,m_vAngles,p1,p2);
	}
}

unsigned int CPlayAreaEntity::GetRoutePoints(){return m_Route.GetPointCount();}
bool CPlayAreaEntity::GetRoutePoint( unsigned int nIndex,SRoutePoint *psPoint ){return m_Route.GetPoint(nIndex,psPoint);}
bool CPlayAreaEntity::AddRoutePoint( unsigned int nIndex,const SRoutePoint &sPoint ){return m_Route.AddPoint(nIndex,sPoint);}
bool CPlayAreaEntity::SetRoutePoint( unsigned int nIndex,const SRoutePoint &sPoint ){return m_Route.SetPoint(nIndex,sPoint);}
void CPlayAreaEntity::RemoveRoutePoint( unsigned int nIndex ){m_Route.RemovePoint(nIndex);}
void CPlayAreaEntity::ClearRoute(){return m_Route.Clear();}

