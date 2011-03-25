#pragma once
enum EFighterState
{
	eFighterState_Normal=ENTITY_STATE_BASE,
	eFighterState_Falling,
	eFighterState_Crashed
};

class CFighterType: public CEntityTypeBase
{
public:
    IEntity *CreateInstance(IEntity *piParent,unsigned int dwCurrentTime);

    double  m_dMaxRoll;
    double  m_dMaxAngularSpeed;
    double  m_dTimeFirstShotMin;
    double  m_dTimeFirstShotMax;
    double  m_dTimeBetweenShotsMin;
    double  m_dTimeBetweenShotsMax;

    BEGIN_PROP_MAP(CFighterType)
        PROP_CLASS_CHAIN(CEntityTypeBase)
        PROP_VALUE_FLAGS(m_dMaxRoll,"MaxRoll",40,MRPF_NORMAL|MRPF_OPTIONAL);
        PROP_VALUE_FLAGS(m_dMaxAngularSpeed,"MaxAngularVelocity",60,MRPF_NORMAL|MRPF_OPTIONAL);
        PROP_VALUE_FLAGS(m_dTimeFirstShotMin,"TimeFirstShotMin",500,MRPF_NORMAL|MRPF_OPTIONAL);
        PROP_VALUE_FLAGS(m_dTimeFirstShotMax,"TimeFirstShotMax",5000,MRPF_NORMAL|MRPF_OPTIONAL);
        PROP_VALUE_FLAGS(m_dTimeBetweenShotsMin,"TimeBetweenShotsMin",2000,MRPF_NORMAL|MRPF_OPTIONAL);
        PROP_VALUE_FLAGS(m_dTimeBetweenShotsMax,"TimeBetweenShotsMax",5000,MRPF_NORMAL|MRPF_OPTIONAL);
    END_PROP_MAP();
	
	BEGIN_ENTITY_STATE_MAP()
		ENTITY_STATE_CHAIN(CEntityTypeBase)
		ENTITY_STATE(eFighterState_Falling,"Falling")
		ENTITY_STATE(eFighterState_Crashed,"Crashed")
	END_ENTITY_STATE_MAP()
	
    CFighterType();
    ~CFighterType();
};


class CFighter: public CEntityBase
{
    CFighterType  *m_pType;

    int m_nRoutePoint;

    double m_dExitYaw;
    double m_dExitPitch;
    double m_dwNextShotTime;

public:

    void Render(IGenericRender *piRender,IGenericCamera *piCamera);
    void ProcessFrame(unsigned int dwCurrentTime,double dTimeFraction);

    void OnKilled();
    bool OnCollision(IEntity *pOther,CVector &vCollisionPos);
    IEntity *GetTarget();

    CFighter(CFighterType *pType,unsigned int dwCurrentTime);
};