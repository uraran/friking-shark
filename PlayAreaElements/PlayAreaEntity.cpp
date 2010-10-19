#include "StdAfx.h"
#include "GameGraphics.h"
#include ".\PlayAreaElementBase.h"
#include ".\PlayAreaEntity.h"

CPlayAreaEntity::CPlayAreaEntity()
{
	m_dRadius=0;
    m_piEntity=NULL;
}

CPlayAreaEntity::~CPlayAreaEntity()
{
    m_EntityType.Detach();
}

bool CPlayAreaEntity::ProcessFrame(CVector vPlayPosition,SPlayAreaInfo *pAreaInfo,DWORD dwCurrentTime,double dInterval)
{
	if(m_EntityType.m_piEntityType && m_dRadius==0){m_dRadius=m_EntityType.m_piEntityType->DesignGetRadius();}

	bool bCurrentlyInPlayArea=Util_IsInPlayArea(m_vPosition,pAreaInfo);
	bCurrentlyInPlayArea=bCurrentlyInPlayArea||Util_IsInPlayArea(m_vPosition-CVector(m_dRadius,0,0),pAreaInfo);
	bCurrentlyInPlayArea=bCurrentlyInPlayArea||Util_IsInPlayArea(m_vPosition+CVector(m_dRadius,0,0),pAreaInfo);
    if(!m_bActive && bCurrentlyInPlayArea)
    {
        Activate(dwCurrentTime);
    }
    else if(m_bActive && !bCurrentlyInPlayArea)
    {
        Deactivate();
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

void CPlayAreaEntity::Activate(DWORD dwCurrentTime)
{
    CPlayAreaElementBase::Activate(dwCurrentTime);

    if(m_EntityType.m_piEntityType!=NULL)
    {
        m_piEntity=m_EntityType.m_piEntityType->CreateInstance(NULL,dwCurrentTime);
        m_piEntity->GetPhysicInfo()->vPosition=m_vPosition;
        m_piEntity->GetPhysicInfo()->vAngles=m_vAngles;
        SUBSCRIBE_TO(m_piEntity,IEntityEvents);
    }
}

void CPlayAreaEntity::Deactivate()
{
    if(m_piEntity)
    {
        UNSUBSCRIBE_FROM_CAST(m_piEntity,IEntityEvents);
        m_piEntity->Remove();
        m_piEntity=NULL;
    }
    CPlayAreaElementBase::Deactivate();
}

void CPlayAreaEntity::OnKilled(IEntity *pUnit)
{
    UNSUBSCRIBE_FROM_CAST(m_piEntity,IEntityEvents);
    m_piEntity=NULL;
}

void CPlayAreaEntity::OnRemoved(IEntity *pUnit)
{
    UNSUBSCRIBE_FROM_CAST(m_piEntity,IEntityEvents);
    m_piEntity=NULL;
}

void CPlayAreaEntity::SetPosition(const CVector &vPosition){m_vPosition=vPosition;}
void CPlayAreaEntity::SetAngles(const CVector &vAngles){m_vAngles=vAngles;}
void CPlayAreaEntity::SetEntityType(IEntityType *piEntityType)
{
	m_EntityType.Attach(piEntityType);
	if(m_EntityType.m_piEntityType){m_dRadius=m_EntityType.m_piEntityType->DesignGetRadius();}
}

CVector CPlayAreaEntity::GetPosition(){return m_vPosition;}
CVector CPlayAreaEntity::GetAngles(){return m_vAngles;}
void	CPlayAreaEntity::GetEntityType(IEntityType **piEntityType){if(piEntityType){*piEntityType=ADD(m_EntityType.m_piEntityType);}	}

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