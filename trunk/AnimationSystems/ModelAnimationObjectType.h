#pragma once

#include "GameGraphics.h"

class CModelAnimationObjectType: public CAnimationObjectTypeBase
{
public:

	CGenericShaderWrapper   m_ShaderWrapper;
	CGenericModelWrapper    m_ModelWrapper;

    float       m_dFps;
    DWORD       m_dwStartTime;
    DWORD       m_dwEndTime;
    bool        m_bCyclic;
    bool        m_bCastShadow;

    IAnimationObject *CreateInstance(IAnimation *piAnimation,DWORD dwCurrentTime);

	void DesignRender(IGenericRender *pRender,CVector &vPosition,CVector &vAngles,bool bSelected);
	void DesignGetBBox(CVector *pvMins,CVector *pvMaxs);
	double DesignGetRadius();
	CTraceInfo DesignGetTrace(const CVector &vOrigin,const CVector &vAngles,const CVector &p1,const CVector &p2 );

    BEGIN_PROP_MAP(CModelAnimationObjectType)
        PROP_CLASS_CHAIN(CAnimationObjectTypeBase)
        PROP(m_ModelWrapper,"Modelo")
        PROP_FLAGS(m_ShaderWrapper,"Shader",MRPF_NORMAL|MRPF_OPTIONAL)
        PROP_VALUE_FLAGS(m_bCastShadow,"EmiteSombras",true,MRPF_NORMAL|MRPF_OPTIONAL)
        PROP_VALUE_FLAGS(m_dFps,"Fps",50.0,MRPF_NORMAL|MRPF_OPTIONAL)
        PROP_VALUE_FLAGS(m_bCyclic,"Repetir",false,MRPF_NORMAL|MRPF_OPTIONAL)
        PROP_VALUE_FLAGS(m_dwStartTime,"TiempoInicio",0,MRPF_NORMAL|MRPF_OPTIONAL)
        PROP_VALUE_FLAGS(m_dwEndTime,"TiempoFin",0,MRPF_NORMAL|MRPF_OPTIONAL)
   END_PROP_MAP();

    CModelAnimationObjectType();
    ~CModelAnimationObjectType();
};


class CModelAnimationObject: public CAnimationObjectBase
{
    CModelAnimationObjectType  *m_pType;
    unsigned int                m_nCurrentFrame;    
    bool                        m_bVisible;

    void Activate(DWORD dwCurrentTime);
    void UpdateVisibility(DWORD dwCurrentTime);
    void Render(IGenericRender *piRender,IGenericCamera *piCamera);

public:

    bool ProcessFrame(IPhysicManager *pPhysicManager,DWORD dwCurrentTime,double dInterval);
    void CustomRender(IGenericRender *piRender,IGenericCamera *piCamera);

	CTraceInfo GetTrace(const CVector &vOrigin,const CVector &vAngles,const CVector &p1,const CVector &p2 );

    CModelAnimationObject(CModelAnimationObjectType *pType,IAnimation *piAnimation);
};
