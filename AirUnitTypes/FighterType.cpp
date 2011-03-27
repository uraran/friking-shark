#include "./stdafx.h"
#include "FighterType.h"
#include "GameGraphics.h"

CFighterType::CFighterType()
{
  m_dMaxRoll=0;
  m_dMaxAngularSpeed=0;
  m_dTimeFirstShotMin=0;
  m_dTimeFirstShotMax=0;
  m_dTimeBetweenShotsMin=0;
  m_dTimeBetweenShotsMax=0;
  m_nDamageType=DAMAGE_TYPE_NORMAL;
  m_nMovementType=PHYSIC_MOVE_TYPE_FLY;
}

CFighterType::~CFighterType(){}

IEntity *CFighterType::CreateInstance(IEntity *piParent,unsigned int dwCurrentTime)
{
  CFighter *piEntity=new CFighter(this,dwCurrentTime);
  InitializeEntity(piEntity,dwCurrentTime);
  piEntity->SetState(eFighterState_Normal);
  return piEntity;
}

CFighter::CFighter(CFighterType *pType,unsigned int dwCurrentTime)
{
  m_sClassName="CFighter";
  m_pType=pType;
  m_nRoutePoint=0;
  m_dwNextProcessFrame=dwCurrentTime+10;
  m_dwNextShotTime=dwCurrentTime+drand()*(m_pType->m_dTimeFirstShotMax-m_pType->m_dTimeFirstShotMin)+m_pType->m_dTimeFirstShotMin;
}

void CFighter::OnKilled()
{
  bool bRemove=false;
  if(m_pTypeBase->GetStateAnimations(eFighterState_Falling))
  {
	if(GetState()!=eFighterState_Falling)
    {
      m_PhysicInfo.vAngleVelocity.c[2]+=drand()*300.0-150.0;
	  SetState(eFighterState_Falling);
      m_PhysicInfo.dwMoveType=PHYSIC_MOVE_TYPE_NORMAL;
    }
  }
  else
  {
    m_PhysicInfo.dwBoundsType=PHYSIC_BOUNDS_TYPE_NONE;
    bRemove=true;
  }
  CEntityBase::OnKilledInternal(bRemove);
}

bool CFighter::OnCollision(IEntity *piOther,CVector &vCollisionPos)
{
	if(GetState()==eFighterState_Falling && piOther->GetAlignment()!=ENTITY_ALIGNMENT_PLAYER)
	{
		if(GetState()!=eFighterState_Crashed && m_pTypeBase->GetStateAnimations(eFighterState_Crashed))
		{
			SetState(eFighterState_Crashed);
		}
		Remove();
	} 
	else if(m_dHealth>0 && piOther->GetAlignment()!=m_dwAlignment)
	{
		piOther->OnDamage(m_dHealth,this);
	}
  return false;
}

void CFighter::ProcessFrame(unsigned int dwCurrentTime,double dTimeFraction)
{
	CEntityBase::ProcessFrame(dwCurrentTime,dTimeFraction);

	if(!m_piRoute){return;}
	if(m_dHealth<=0)
	{
		if(GetState()==eFighterState_Falling)
		{
			//m_PhysicInfo.vAngles.c[1]=ApproachAngle(m_PhysicInfo.vAngles.c[1],40,30);
		}
		return;
	}

	m_dwNextProcessFrame=dwCurrentTime+10;

	CVector vForward,vRight,vUp;
	VectorsFromAngles(m_PhysicInfo.vAngles.c[YAW],m_PhysicInfo.vAngles.c[PITCH],0,vForward,vRight,vUp);
	CVector vDest=m_piRoute->GetAbsolutePoint(m_piRoute->GetNextPointIndex(m_nRoutePoint));
	CVector vDir=vDest-m_PhysicInfo.vPosition;
	double dDist=vDir.N();  
	
	bool bNext=false;
	int nNext=m_piRoute->GetNextPointIndex(m_nRoutePoint);
	if(nNext!=m_nRoutePoint)
	{
		CVector vDirNext=m_piRoute->GetAbsolutePoint(m_piRoute->GetNextPointIndex(nNext))-vDest;
		vDirNext.N();
		
		double dCirclePerimeter=(m_PhysicInfo.dMaxVelocity*360.0/m_pType->m_dMaxAngularSpeed);
		double dCapableRadius=(dCirclePerimeter/(2*PI));
		
		CVector vPerpB=vDirNext^vUp;
		CVector vPB1=vDest+vDirNext*dDist;
		CPlane  vPlaneA=CPlane(vDir,m_PhysicInfo.vPosition);
		
		double dSide1=vPlaneA.GetSide(vPB1);
		double dSide2=vPlaneA.GetSide(vPB1+vPerpB*10000.0);
		double dLength=(dSide1-dSide2);
		double dFraction=dLength?dSide1/dLength:0;
		double dFinalRadius=fabs(10000.0*dFraction);

		bNext=(dFinalRadius<dCapableRadius);
	}
	bNext=(bNext || dDist<m_PhysicInfo.dMaxVelocity*0.1);
	if(bNext)
	{
		if(nNext==m_nRoutePoint)
		{
			Remove();
		}
		else
		{
			m_nRoutePoint=nNext;
		}
	}
	
	if(vForward!=vDir)
	{
		CPlane plane(vRight,m_PhysicInfo.vPosition);
		if(plane.GetSide(vDest)>0)
		{
			m_PhysicInfo.vAngles.c[ROLL]+=m_pType->m_dMaxRoll*dTimeFraction;
			if(m_PhysicInfo.vAngles.c[ROLL]>m_pType->m_dMaxRoll){m_PhysicInfo.vAngles.c[ROLL]=m_pType->m_dMaxRoll;}
		}
		else
		{
			m_PhysicInfo.vAngles.c[ROLL]-=m_pType->m_dMaxRoll*dTimeFraction;
			if(m_PhysicInfo.vAngles.c[ROLL]<-m_pType->m_dMaxRoll){m_PhysicInfo.vAngles.c[ROLL]=-m_pType->m_dMaxRoll;}
		}
	}
	else
	{
		if(m_PhysicInfo.vAngles.c[ROLL]>0)
		{
			m_PhysicInfo.vAngles.c[ROLL]-=m_pType->m_dMaxAngularSpeed*dTimeFraction;
			if(m_PhysicInfo.vAngles.c[ROLL]<0){m_PhysicInfo.vAngles.c[ROLL]=0;}
		}
		else
		{
			m_PhysicInfo.vAngles.c[ROLL]+=m_pType->m_dMaxAngularSpeed*dTimeFraction;
			if(m_PhysicInfo.vAngles.c[ROLL]>0){m_PhysicInfo.vAngles.c[ROLL]=0;}
		}
	}	

	double dDesiredYaw=0,dDesiredPitch=0;
	AnglesFromVector(vDir,dDesiredYaw,dDesiredPitch);
	double dCurrentAngularSpeed=m_pType->m_dMaxAngularSpeed;
	m_PhysicInfo.vAngles.c[YAW]=ApproachAngle(m_PhysicInfo.vAngles.c[YAW],dDesiredYaw,-dCurrentAngularSpeed*dTimeFraction);
	VectorsFromAngles(m_PhysicInfo.vAngles,&m_PhysicInfo.vVelocity);
	m_PhysicInfo.vVelocity*=m_PhysicInfo.dMaxVelocity;
	m_dwNextProcessFrame=dwCurrentTime+10;

	if(dwCurrentTime>m_dwNextShotTime)
	{
		FireWeapon(0,dwCurrentTime);
		m_dwNextShotTime=dwCurrentTime+drand()*(m_pType->m_dTimeBetweenShotsMax-m_pType->m_dTimeBetweenShotsMin)+m_pType->m_dTimeBetweenShotsMin;
	}
}

void CFighter::Render(IGenericRender *piRender,IGenericCamera *piCamera)
{
  CEntityBase::Render(piRender,piCamera);
  
  // Render route
  /*
  if(m_piRoute)
  {
	piRender->PushState();
	piRender->DeactivateDepth();
	
	CVector vPreviousPoint=m_piRoute->GetAbsolutePoint(0);
	for(unsigned x=1;x<m_piRoute->GetPointCount();x++)
	{
		double dPointSize=15;
		CVector vPointColor=CVector(0,0,0.8);
		CVector vLineColor=CVector(0,0,0.8);
		
		
		CVector vWorld=m_piRoute->GetAbsolutePoint(x);
		piRender->RenderLine(vPreviousPoint,vWorld,vLineColor);
		piRender->RenderPoint(vWorld,dPointSize,vPointColor,1.0);
		vPreviousPoint=vWorld;
	
	}
	piRender->PopState();
  }*/
}

IEntity *CFighter::GetTarget()
{
  if(m_piTarget==NULL)
  {
    IEntityManager *piManager=GetEntityManager();
    if(piManager)
    {
      m_piTarget=piManager->FindEntity("Player");
    }
  }
  return m_piTarget;
}
 
