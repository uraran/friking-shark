#include "./StdAfx.h"
#include "VectorLib.h"
#include "3DSTypes.h"
#include <string>
using namespace std;

S3DSScene::S3DSScene()
{
	dFirstFrame=0;
	dLastFrame=0;
	dFrameSpeed=0;
	dTicksPerFrame=0;
	nFrameCount=0;
}

S3DSFrame::S3DSFrame()
{
	dTimeStamp=0;
	nFaces=0;
	nVertexes=0;
	nTextVertexes=0;
	nColorVertexes=0;
	nTextFaces=0;
	nColorFaces=0;
	pFaces=NULL;
	pFaceSubMaterials=NULL;
	pVertexes=NULL;
	pVertexNormals=NULL;
	pTextVertexes=NULL;
	pTextFaces=NULL;
	pColorVertexes=NULL;
	pColorFaces=NULL;
	pEdges=NULL;
}

S3DSFrame::~S3DSFrame()
{
	if(pFaces){delete [] pFaces;pFaces=NULL;}
	if(pFaceSubMaterials){delete [] pFaceSubMaterials;pFaceSubMaterials=NULL;}
	if(pVertexNormals){delete [] pVertexNormals;pVertexNormals=NULL;}
	if(pVertexes){delete [] pVertexes;pVertexes=NULL;}
	if(pTextVertexes){delete [] pTextVertexes;pTextVertexes=NULL;}
	if(pTextFaces){delete [] pTextFaces;pTextFaces=NULL;}
	if(pColorVertexes){delete [] pColorVertexes;pColorVertexes=NULL;}
	if(pColorFaces){delete [] pColorFaces;pColorFaces=NULL;}
	if(pEdges){delete [] pEdges;pEdges=NULL;}
	unsigned int y;
	for(y=0;y<sObjectMaterials.size();y++)
	{
		S3DSObjectMaterial *pObjectMaterial=sObjectMaterials[y];
		delete pObjectMaterial;
	}
}


S3DSMaterial::S3DSMaterial()
{
	fShininess=0;
	fShininessStrength=0;
	fTranparency=0;
	dwMaterialType=MATERIAL_SOLID;
	sName[0]=0;
	sFile[0]=0;
	sAlphaFile[0]=0;
	vAmbientColor=CVector(0.5,0.5,0.5);
	vDiffuseColor=CVector(0.5,0.5,0.5);
	vSpecularColor=CVector(0.5,0.5,0.5);
	bTwoSided=false;

	bSubMaterial=false; // Solo para ASE
	dwMaterialId=-1;
	dwSubMaterialId=0;

	fTextureUOffset=0;
	fTextureVOffset=0;
	fTextureUScale=0;
	fTextureVScale=0;
	fTextureAngle=0;
}

S3DSObjectMaterial::S3DSObjectMaterial()
{
	sName[0]=0;
	nFaces=0;
	pFaces=NULL;
}

S3DSObjectMaterial::~S3DSObjectMaterial()
{
	if(pFaces){delete [] pFaces;}
}

S3DSTextureFace::S3DSTextureFace()
{
	nFaceIndex=-1;
	memset(pTextVertexes,0,sizeof(pTextVertexes));
}

S3DSColorFace::S3DSColorFace()
{
	nFaceIndex=-1;
	memset(pColorVertexes,0,sizeof(pColorVertexes));
}

S3DSObject::S3DSObject()
{
	sName[0]=0;
	bVisible=true;
	dwMaterialId=-1;
	bHasFirstFrameInfo=false;
	fFirstFrameRotationAngle=0;
	vWireframeColor=CVector(0.5,0.5,0.5);
}

S3DSObject::~S3DSObject()
{
	unsigned int y;
	for(y=0;y<vFrameInstances.size();y++)
	{
		S3DSFrame *pFrame=vFrameInstances[y];
		delete pFrame;
	}
}


S3DSLight::S3DSLight()
{
	vColor=CVector(1,1,1);
	sName[0]=0;
	dwType=S3DS_LIGHT_TYPE_OMNI;
	bIsOff=false;
	bAttenuationOn=false;
	fAttenuationStart=0;
	fAttenuationEnd=0;
	fSpotHotSpot=0;
	fSpotFallOff=0;
	fMultiplier=1;
	bHasTarget=false;
	fTargetDist=0;
	fRotationAngle=0;
}

S3DSCamera::S3DSCamera()
{
	sName[0]=0;
	fRoll=0;
	fLens=0;
}


CVector From3DSToOpenGL(CVector v)
{
	return From3DSToOpenGL(&v);
}

CVector From3DSToOpenGL(CVector *pV)
{
	double temp=pV->c[2];
	pV->c[2]=-pV->c[1];
	pV->c[1]=temp;
	return *pV;
}

CVector FromOpenGLTo3DS(CVector *pV)
{
	double temp=pV->c[1];
	pV->c[1]=-pV->c[2];
	pV->c[2]=temp;
	return *pV;
}

CVector FromOpenGLTo3DS(CVector v)
{
	return FromOpenGLTo3DS(&v);
}



C3DSFileTypeBase::C3DSFileTypeBase()
{
}

C3DSFileTypeBase::~C3DSFileTypeBase()
{
	Close();
}

S3DSObject *C3DSFileTypeBase::FindObject(const char *sFileName)
{
	string tempName=sFileName;
	S3DSObject *pObject=NULL;
	for(unsigned int x=0;x<m_vObjects.size();x++)
	{
		if(m_vObjects[x]->sName==tempName){return m_vObjects[x];}
	}
	return pObject;
}

void C3DSFileTypeBase::Close()
{
	unsigned int x;
	for(x=0;x<m_vObjects.size();x++)
	{
		S3DSObject *pObject=m_vObjects[x];
		delete pObject;
	}
	for(x=0;x<m_vMaterials.size();x++)
	{
		S3DSMaterial *pMaterial=m_vMaterials[x];
		delete pMaterial;
	}
	for(x=0;x<m_vLights.size();x++)
	{
		S3DSLight *pLight=m_vLights[x];
		delete pLight;
	}
	for(x=0;x<m_vCameras.size();x++)
	{
		S3DSCamera *pCamera=m_vCameras[x];
		delete pCamera;
	}
	m_vObjects.clear();
	m_vMaterials.clear();
	m_vLights.clear();
	m_vCameras.clear();
}