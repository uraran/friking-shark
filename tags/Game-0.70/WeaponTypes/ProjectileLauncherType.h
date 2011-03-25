#pragma once

#define eProjectileLauncherReferenceSystem_Unknown    0
#define eProjectileLauncherReferenceSystem_Owner      1
#define eProjectileLauncherReferenceSystem_Target     2

struct SProjectileLauncherProjectile
{
  CVector     vOrigin;
  CVector     vDirection;
  double      dVelocity;
  CEntityTypeWrapper projectileEntityType;
  unsigned int       dwReferenceSystem;

  SProjectileLauncherProjectile(){}
};

BEGIN_STRUCT_PROPS(SProjectileLauncherProjectile)
  PROP(projectileEntityType,"Entidad")
  PROP_VALUE_FLAGS(vOrigin,"Origen",Origin,MRPF_NORMAL|MRPF_OPTIONAL)
  PROP_VALUE_FLAGS(vDirection,"Direccion",CVector(1,0,0),MRPF_NORMAL|MRPF_OPTIONAL)
  PROP_VALUE_FLAGS(dVelocity,"Velocidad",CVector(1,0,0),MRPF_NORMAL|MRPF_OPTIONAL)
  PROP_VALUE_FLAGS(dwReferenceSystem,"SistemaDeReferencia",eProjectileLauncherReferenceSystem_Owner,MRPF_NORMAL|MRPF_OPTIONAL)
END_STRUCT_PROPS()

struct SProjectileLauncherLevel
{
  double                                dRange;
  unsigned int                                 dwReloadTime;
  deque<SProjectileLauncherProjectile>  dProjectiles;

  SProjectileLauncherLevel(){dRange=0;dwReloadTime=1000;}
};

BEGIN_STRUCT_PROPS(SProjectileLauncherLevel)
  PROP(dwReloadTime,"TiempoDeRecarga")
  PROP(dProjectiles,"Proyectiles")
  PROP_VALUE_FLAGS(dRange,"Alcance",0,MRPF_NORMAL|MRPF_OPTIONAL)
END_STRUCT_PROPS()

class CProjectileLauncherType: virtual public CSystemObjectBase,virtual public IWeaponType
{
  bool m_bProjectileEntityTypesResolved;

  unsigned int                           m_dwWeaponSlot;
  deque<SProjectileLauncherLevel> m_dLevels;

public:

  bool m_bIgnoreRoll;

  IWeapon *CreateInstance(IEntity *piEntity,unsigned int dwCurrentTime);

  unsigned int                     GetSlot();
  unsigned int                     GetMaxLevel();
  SProjectileLauncherLevel *GetLevel(unsigned int dwLevel);
 
  BEGIN_PROP_MAP(CProjectileLauncherType)
    PROP_FLAGS(m_dLevels,"Niveles",MRPF_NORMAL|MRPF_OPTIONAL)
    PROP_VALUE_FLAGS(m_dwWeaponSlot,"Banco",0,MRPF_NORMAL|MRPF_OPTIONAL)
    PROP_VALUE_FLAGS(m_bIgnoreRoll,"IgnorarAnguloRoll",true,MRPF_NORMAL|MRPF_OPTIONAL)
  END_PROP_MAP()

  CProjectileLauncherType();
  ~CProjectileLauncherType();
};

class CProjectileLauncher:public IWeapon
{
  CProjectileLauncherType  *m_pType;
  SProjectileLauncherLevel *m_pCurrentLevel;
  IEntity                  *m_piEntity;
   unsigned int                     m_dwLastFireTime;
  unsigned int                     m_dwCurrentLevel;

public:
  
  // IWeapon

  unsigned int GetSlot();

  void Fire(unsigned int dwCurrentTime);
  bool IsReady(unsigned int dwCurrentTime);

  unsigned int GetCurrentLevel();
  void  SetCurrentLevel(unsigned int dwLevel);

  CProjectileLauncher(CProjectileLauncherType *pType,IEntity *piEntity,unsigned int dwCurrentTimeBase);
  ~CProjectileLauncher();
};