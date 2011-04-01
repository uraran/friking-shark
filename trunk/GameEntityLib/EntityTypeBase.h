#pragma once


DECLARE_CUSTOM_WRAPPER1(CAnimationTypeWrapper,IAnimationType,m_piAnimationType)
DECLARE_CUSTOM_WRAPPER1(CWeaponTypeWrapper,IWeaponType,m_piWeaponType)
DECLARE_CUSTOM_WRAPPER1(CChildEntityTypeWrapper,IEntityType,m_piEntityType)

class CEntityBase;

struct SEntityState
{
	std::string 					sName;
	vector<CAnimationTypeWrapper>  	vAnimations;
};

struct SChildEntityType
{
	CChildEntityTypeWrapper entityType;
	CVector vPosition;
	CVector vAngles;
};

BEGIN_STRUCT_PROPS(SChildEntityType)
	PROP_FLAGS(entityType  ,"EntityType",MRPF_NORMAL|MRPF_OPTIONAL)
	PROP_FLAGS(vPosition   ,"Position",MRPF_NORMAL|MRPF_OPTIONAL)
	PROP_FLAGS(vAngles     ,"Angles",MRPF_NORMAL|MRPF_OPTIONAL)
END_STRUCT_PROPS()

#define BEGIN_ENTITY_STATE_MAP() virtual void RegisterStates(){
#define ENTITY_STATE(index,name) if(m_vStates.size()<=index){m_vStates.resize(index+1);} m_vStates[index].sName=name;
#define ENTITY_STATE_CHAIN(toclass) toclass::RegisterStates();
#define END_ENTITY_STATE_MAP() }

class CEntityTypeBase: virtual public CSystemObjectBase,virtual public IEntityType,virtual public IEntityTypeDesign
{
private:

    // Propiedades Persistentes
	map<std::string,vector<CAnimationTypeWrapper> >  m_mStateAnimations;
	vector<CWeaponTypeWrapper>     					 m_vWeapons;
	vector<SChildEntityType>     					 m_vChildren;

	void MapStateAnimations();
	void InitializeStates();
	CAnimationTypeWrapper *GetStateAnimation(unsigned int nState,unsigned int nIndex);
protected:
	
	CVector      m_vBBoxMins;
	CVector      m_vBBoxMaxs;
	unsigned int m_nDamageType;
	unsigned int m_nBoundsType;
	unsigned int m_nMovementType;
	unsigned int m_nCollisionType;
	unsigned int m_nAlignment;
	double		 m_dMaxHealth;
	double		 m_dMaxVelocity;
	
	vector<SEntityState> m_vStates;
	
    virtual void	InitializeEntity(CEntityBase *piEntity,unsigned int dwCurrentTime);

	virtual void	GetBBox(CVector *pvMins,CVector *pvMaxs);
	virtual CVector	GetSize();
	virtual void	DesignRender(IGenericRender *piRender,CVector &vPosition,CVector &vAngles,bool bSelected);
	virtual void    DesignGetBBox(CVector *pvMins,CVector *pvMaxs);
	virtual double  DesignGetRadius();
	virtual CTraceInfo DesignGetTrace(const CVector &vPosition,const CVector &vAngles,const CVector &p1,const CVector &p2 );

public:

	bool Init(std::string sClass,std::string sName,ISystem *piSystem);
	bool Unserialize(ISystemPersistencyNode *piNode);

    BEGIN_PROP_MAP(CEntityTypeBase);
        PROP(m_mStateAnimations ,"StateAnimations")
		PROP_FLAGS(m_vWeapons   ,"Weapons",MRPF_NORMAL|MRPF_OPTIONAL)
		PROP_FLAGS(m_vChildren  ,"Children",MRPF_NORMAL|MRPF_OPTIONAL)
		PROP_FLAGS(m_vBBoxMins  ,"BBoxMins",MRPF_NORMAL|MRPF_OPTIONAL)
		PROP_FLAGS(m_vBBoxMaxs  ,"BBoxMaxs",MRPF_NORMAL|MRPF_OPTIONAL)
		PROP_FLAGS(m_nMovementType,"MovementType",MRPF_NORMAL|MRPF_OPTIONAL)
		PROP_FLAGS(m_nCollisionType,"CollisionType",MRPF_NORMAL|MRPF_OPTIONAL)
		PROP_FLAGS(m_nBoundsType,"BoundsType",MRPF_NORMAL|MRPF_OPTIONAL)
		PROP_FLAGS(m_nDamageType,"DamageType",MRPF_NORMAL|MRPF_OPTIONAL)
		PROP_FLAGS(m_nAlignment ,"Alignment",MRPF_NORMAL|MRPF_OPTIONAL)
		PROP_FLAGS(m_dMaxHealth ,"MaxHealth",MRPF_NORMAL|MRPF_OPTIONAL)
		PROP_FLAGS(m_dMaxVelocity,"MaxVelocity",MRPF_NORMAL|MRPF_OPTIONAL)
	END_PROP_MAP();

	BEGIN_ENTITY_STATE_MAP()
		ENTITY_STATE(ENTITY_STATE_BASE,"BaseState")
	END_ENTITY_STATE_MAP()

	IAnimation  *CreateStateAnimation(CEntityBase *pEntity,unsigned int nState,unsigned int nIndex,unsigned int nCurrentTime);
	unsigned int GetStateAnimations(unsigned int nState);
	
	// IEntityTypeDesign
	
	void		 GetEntityTypeConfig(SEntityTypeConfig *pConfig);
	void		 SetEntityTypeConfig(SEntityTypeConfig *pConfig);
	
	// State Management
	
	unsigned int GetStateCount();
	bool		 GetStateName(unsigned int nIndex,std::string *psName);
	
	// Animation management
	
	unsigned int	AddStateAnimation(unsigned int nState);
	bool			RemoveStateAnimation(unsigned int nState,unsigned int nAnimation);
	bool			GetStateAnimation(unsigned int nState,unsigned int nAnimation,IAnimationType **ppiAnimation);
	unsigned int	GetStateAnimationCount(unsigned int nState);
	
	// Weapon management
	
	unsigned int	AddWeapon(std::string sWeaponType);
	bool			RemoveWeapon(unsigned int nWeapon);
	bool			GetWeapon(unsigned int nWeapon,IWeaponType **ppiWeapon);
	unsigned int	GetWeaponCount(unsigned int nState);
	
	// Children management
	
	unsigned int	AddChild(std::string sEntity);
	bool			RemoveChild(unsigned int nChild);
	bool			GetChild(unsigned int nChild,IEntityType **ppiEntityType);
	unsigned int	GetChildren();
	void			SetChildLocation(unsigned int nChild,CVector vPosition,CVector vAngles);
	void			GetChildLocation(unsigned int nChild,CVector &vPosition,CVector &vAngles);
	
	CEntityTypeBase(void);
    ~CEntityTypeBase(void);
};