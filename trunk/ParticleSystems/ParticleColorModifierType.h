#pragma once

struct SColorTransition
{
    double  dTime;// de 0 a 1, en tiempo de vida de la particula
    CRGBColor value;
};

struct SAlphaTransition
{
    double  dTime;// de 0 a 1, en tiempo de vida de la particula
    double  value;
};

BEGIN_STRUCT_PROPS(SColorTransition)
    PROP(dTime,"Tiempo")
    PROP(value,"Valor")
END_STRUCT_PROPS()

BEGIN_STRUCT_PROPS(SAlphaTransition)
    PROP(dTime,"Tiempo")
    PROP(value,"Valor")
END_STRUCT_PROPS()

class CParticleColorModifierType:public CParticleModifierType
{
public:

    // Propiedades Persistentes

    CRGBColor               m_vStartColor;
    CRGBColor               m_vEndColor;
    double                  m_dStartAlpha;
    double                  m_dEndAlpha;
    double                  m_dColorTransitionStart;// de 0 a 1, en tiempo de vida de la particula
    double                  m_dColorTransitionEnd;  // de 0 a 1, en tiempo de vida de la particula
    double                  m_dAlphaTransitionStart;// de 0 a 1, en tiempo de vida de la particula
    double                  m_dAlphaTransitionEnd;  // de 0 a 1, en tiempo de vida de la particula
    deque<SColorTransition> m_dIntermediateColorTransitions;
    deque<SAlphaTransition> m_dIntermediateAlphaTransitions;


    IParticleModifier *CreateInstance(DWORD dwCurrentTime);

    BEGIN_PROP_MAP(CParticleColorModifierType);
        PROP_CLASS_CHAIN(CParticleModifierType);
        PROP_VALUE_FLAGS(m_vStartColor,"ColorInicial",CRGBColor(1,1,1),MRPF_NORMAL|MRPF_OPTIONAL)
        PROP_VALUE_FLAGS(m_vEndColor,"ColorFinal",CRGBColor(1,1,1),MRPF_NORMAL|MRPF_OPTIONAL)
        PROP_VALUE_FLAGS(m_dStartAlpha,"AlphaInicial",1,MRPF_NORMAL|MRPF_OPTIONAL)
        PROP_VALUE_FLAGS(m_dEndAlpha,"AlphaFinal",1,MRPF_NORMAL|MRPF_OPTIONAL)
        PROP_VALUE_FLAGS(m_dColorTransitionStart,"InicioTransicionColor",0,MRPF_NORMAL|MRPF_OPTIONAL)
        PROP_VALUE_FLAGS(m_dColorTransitionEnd,"FinTransicionColor",1,MRPF_NORMAL|MRPF_OPTIONAL)
        PROP_VALUE_FLAGS(m_dAlphaTransitionStart,"InicioTransicionAlpha",0,MRPF_NORMAL|MRPF_OPTIONAL)
        PROP_VALUE_FLAGS(m_dAlphaTransitionEnd,"FinTransicionAlpha",1,MRPF_NORMAL|MRPF_OPTIONAL)
        PROP_FLAGS(m_dIntermediateColorTransitions,"TransicionesIntermediasColor",MRPF_NORMAL|MRPF_OPTIONAL)
        PROP_FLAGS(m_dIntermediateAlphaTransitions,"TransicionesIntermediasAlpha",MRPF_NORMAL|MRPF_OPTIONAL)
    END_PROP_MAP();

    CParticleColorModifierType();
    ~CParticleColorModifierType();
};

class CParticleColorModifier:public CParticleModifier
{
    CParticleColorModifierType *m_pType;
public:

    void ProcessParticle(IParticle *pParticle,IParticleSystem *pSystem,DWORD dwCurrentTime,double dInterval);

    CParticleColorModifier(CParticleColorModifierType *pType);
};