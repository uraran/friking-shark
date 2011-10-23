//  Friking Shark, a Flying Shark arcade remake.
//  Copyright (C) 2011  Javier Martin Garcia (javiermartingarcia@gmail.com)
//	
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//  
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//  
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.
//  


#include "./stdafx.h"
#include "OpenGLGraphics.h"
#include "OpenGLShader.h"

COpenGLShader::COpenGLShader(void)
{
  m_hVertexShader=0;
  m_hFragmentShader=0;
  m_hShaderProgram=0;
  m_bTriedToCompile=false;
  m_bActive=false;
}

COpenGLShader::~COpenGLShader(void)
{	
	std::map<std::string,SUniformData*>::iterator i;
	for(i=m_mUniforms.begin();i!=m_mUniforms.end();i++)
	{
		SUniformData *pData=i->second;
		delete pData;
	}
	m_mUniforms.clear();
}

bool COpenGLShader::LoadCodeFile(std::string sSourceFile,std::string *psSourceCode)
{
	char *pSource=NULL;
	unsigned int dwSize=0;
	bool bOk=false;

	FILE *pFile=fopen(sSourceFile.c_str(),"rb");
	if(pFile!=NULL)
	{
		fseek(pFile,0,SEEK_END);
		dwSize=ftell(pFile);
		fseek(pFile,0,SEEK_SET);
		if(dwSize)
		{
			pSource=new char [dwSize+1];
			if(pSource)
			{
				if(fread(pSource,dwSize,1,pFile)==1)
				{
					pSource[dwSize]=0;
					bOk=true;
				}
			}
		}
	}
	if(pFile){fclose(pFile);pFile=NULL;}
	if(bOk)
	{
		*psSourceCode=pSource;
	}
	if(pSource){delete [] pSource;pSource=NULL;}
	m_bTriedToCompile=false;
	return bOk;
}

bool COpenGLShader::Compile()
{
  bool bOk=true;

  if(m_hShaderProgram==0 && m_bTriedToCompile){return false;}
  
  FreeShader();
  if(m_sFragmentShaderCode.length()==0 && m_sVertexShaderCode.length()==0){return false;}
  m_bTriedToCompile=true;
  
  m_hShaderProgram=glCreateProgramObjectARB();
  bOk=(m_hShaderProgram!=0);

  if(bOk && m_sVertexShaderCode.length())
  {
    m_hVertexShader=glCreateShaderObjectARB(GL_VERTEX_SHADER_ARB);
    if(m_hVertexShader)
    {
      GLint glResult=0,iSize=m_sVertexShaderCode.length();
	  const char *psCode=m_sVertexShaderCode.c_str();
	  if(bOk){glShaderSourceARB(m_hVertexShader,1,const_cast<const GLcharARB**>(&psCode),&iSize);}
      if(bOk)
	  {
		  glCompileShaderARB(m_hVertexShader);
		  glGetObjectParameterivARB(m_hVertexShader,GL_OBJECT_COMPILE_STATUS_ARB,&glResult);
		  bOk=(glResult==1);

		  char sOuputBuffer[1024*16];
		  sOuputBuffer[0]=0;
		  int nLength=0;
		  glGetInfoLogARB(m_hVertexShader,sizeof(sOuputBuffer),&nLength,sOuputBuffer);
		  if(!bOk){RTTRACE("COpenGLShader::Compile -> Vertex Shader compilation Failed");}
		  if(sOuputBuffer[0]!=0){RTTRACE(sOuputBuffer);}
		  //if(sOuputBuffer[0]!=0){RTTRACE(m_sVertexShaderCode.c_str());}
	  }
      if(bOk){glAttachObjectARB(m_hShaderProgram,m_hVertexShader);}
	}
    else
    {
      bOk=false;
    }
  }
  if(bOk && m_sFragmentShaderCode.length())
  {
    m_hFragmentShader=glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);
    if(m_hFragmentShader)
    {
	  GLint glResult=0,iSize=m_sFragmentShaderCode.length();
	  const char *psCode=m_sFragmentShaderCode.c_str();
	  if(bOk){glShaderSourceARB(m_hFragmentShader,1,const_cast<const GLcharARB**>(&psCode),&iSize);}
      if(bOk)
	  {
		  glCompileShaderARB(m_hFragmentShader);
		  glGetObjectParameterivARB(m_hFragmentShader,GL_OBJECT_COMPILE_STATUS_ARB,&glResult);
		  bOk=(glResult==1);

		  char sOuputBuffer[1024*16];
		  sOuputBuffer[0]=0;
		  int nLength=0;
		  glGetInfoLogARB(m_hFragmentShader,sizeof(sOuputBuffer),&nLength,sOuputBuffer);
		  if(!bOk){RTTRACE("COpenGLShader::Compile -> Fragment Shader compilation Failed");}
		  if(sOuputBuffer[0]!=0){RTTRACE(sOuputBuffer);}
		  //if(sOuputBuffer[0]!=0){RTTRACE(m_sFragmentShaderCode.c_str());}
	  }
      if(bOk){glAttachObjectARB(m_hShaderProgram,m_hFragmentShader);}

    }
    else
    {
      bOk=false;
    }
  }
  
  std::map<std::string,SAttributeData>::iterator iAttribute;
  for(iAttribute=m_mAttributes.begin();iAttribute!=m_mAttributes.end();iAttribute++)
  {
	  SAttributeData *pData=&iAttribute->second;
	  glBindAttribLocation(m_hShaderProgram,pData->nIndex,iAttribute->first.c_str());
	  pData->bModified=false;
  }
  
  if(bOk)
  {
    GLint glResult=0;
	glLinkProgramARB(m_hShaderProgram);
    glGetObjectParameterivARB(m_hShaderProgram,GL_OBJECT_LINK_STATUS_ARB,&glResult);
    bOk=(glResult==1);
	char sOuputBuffer[1024*16];
	sOuputBuffer[0]=0;
	int nLength=0;
	glGetInfoLogARB(m_hShaderProgram,sizeof(sOuputBuffer),&nLength,sOuputBuffer);
	if(sOuputBuffer[0]!=0){RTTRACE(sOuputBuffer);}
	RTTRACE(bOk?"COpenGLShader::Compile -> Shader link Succeeded":"COpenGLShader::Compile -> Shader link failed");
  }
  if(!bOk){FreeShader();}
  return bOk;
}

void COpenGLShader::FreeShader()
{
	if(m_hVertexShader){glDetachObjectARB(m_hShaderProgram,m_hVertexShader);glDeleteObjectARB(m_hVertexShader);m_hVertexShader=0;}
	if(m_hFragmentShader){glDetachObjectARB(m_hShaderProgram,m_hFragmentShader);glDeleteObjectARB(m_hFragmentShader);m_hFragmentShader=0;}
	if(m_hShaderProgram){glDeleteObjectARB(m_hShaderProgram);m_hShaderProgram=0;}
	
	std::map<std::string,SUniformData*>::iterator i;
	for(i=m_mUniforms.begin();i!=m_mUniforms.end();i++)
	{
		SUniformData *pData=i->second;
		pData->bModified=!pData->bTemporal;
		pData->nLocation=-1;
	}
}

bool COpenGLShader::Activate()
{  
	if(m_hShaderProgram==0)
	{
	  if(!Compile()){return false;}
	}
  
	glUseProgramObjectARB(m_hShaderProgram);
  
	std::map<std::string,SUniformData*>::iterator iUniform;
	for(iUniform=m_mUniforms.begin();iUniform!=m_mUniforms.end();iUniform++)
	{
		SUniformData *pData=iUniform->second;
		if(pData->nLocation==-1){pData->nLocation=glGetUniformLocationARB(m_hShaderProgram, iUniform->first.c_str());}
		if(!pData->bModified){continue;}
		if(pData->bTemporal){continue;}
		pData->bModified=false;
		
		switch(pData->eType)
		{
			case eUniformType_Int:
				glUniform1iARB(pData->nLocation,pData->data.nInteger);
			break;
			case eUniformType_Float:
				glUniform1fARB(pData->nLocation,pData->data.fFloats[0]);
			break;
			case eUniformType_Vector2:
				glUniform2fvARB(pData->nLocation,1,pData->data.fFloats);
			break;
			case eUniformType_Vector3:
				glUniform3fvARB(pData->nLocation,1,pData->data.fFloats);
			break;
			case eUniformType_Color:
				glUniform4fvARB(pData->nLocation,1,pData->data.fFloats);
			break;
			case eUniformType_Matrix:
				glUniformMatrix4fvARB(pData->nLocation,1,true,pData->data.fFloats);
			break;
			case eUniformType_IntArray:
				glUniform1ivARB(pData->nLocation,pData->nElements,(int*)pData->pBuffer);
			break;
			case eUniformType_FloatArray:
				glUniform1fvARB(pData->nLocation,pData->nElements,(float*)pData->pBuffer);
			break;
			case eUniformType_Vector3Array:
				glUniform3fvARB(pData->nLocation,pData->nElements,(float*)pData->pBuffer);
			break;
			case eUniformType_ColorArray:
				glUniform4fvARB(pData->nLocation,pData->nElements,(float*)pData->pBuffer);
			break;
			case eUniformType_MatrixArray:
				glUniformMatrix4fvARB(pData->nLocation,pData->nElements,true,(float*)pData->pBuffer);
			break;
			default:
				break;
		}
	}
	
	m_bActive=true;
	
	return true;
}

void COpenGLShader::AddUniform( std::string sUniformName,int nValue,bool bTemporal )
{
	std::map<std::string,SUniformData*>::iterator i;
	i=m_mUniforms.find(sUniformName);
	if(i==m_mUniforms.end())
	{
		SUniformData *pData=new SUniformData;
		i=m_mUniforms.insert(std::pair<std::string,SUniformData*>(sUniformName,pData)).first;
	}
	else
	{
		SUniformData *pData=i->second;
		if(pData->eType==eUniformType_Int && pData->data.nInteger==nValue)
		{
			// Not mofidied
			return;
		}
	}
	
	SUniformData *pData=i->second;
	pData->eType=eUniformType_Int;
	pData->data.nInteger=nValue;
	pData->bTemporal=bTemporal;
	pData->bModified=!bTemporal;
	pData->nElements=0;
	if(pData->pBuffer){delete [] pData->pBuffer;pData->pBuffer=NULL;}
	
	if(m_bActive)
	{
		if(pData->nLocation==-1){pData->nLocation=glGetUniformLocationARB(m_hShaderProgram, sUniformName.c_str());}
		if(pData->nLocation!=-1){pData->bModified=false;glUniform1iARB(pData->nLocation,pData->data.nInteger);}
	}
}

void COpenGLShader::AddUniform( std::string sUniformName,float fValue,bool bTemporal )
{
	std::map<std::string,SUniformData*>::iterator i;
	i=m_mUniforms.find(sUniformName);
	if(i==m_mUniforms.end())
	{
		SUniformData *pData=new SUniformData;
		i=m_mUniforms.insert(std::pair<std::string,SUniformData*>(sUniformName,pData)).first;
	}
	else
	{
		SUniformData *pData=i->second;
		if(pData->eType==eUniformType_Float && pData->data.fFloats[0]==fValue)
		{
			// Not mofidied
			return;
		}
	}	  
	SUniformData *pData=i->second;
	pData->eType=eUniformType_Float;
	pData->data.fFloats[0]=fValue;
	pData->bTemporal=bTemporal;
	pData->bModified=!bTemporal;
	pData->nElements=0;
	if(pData->pBuffer){delete [] pData->pBuffer;pData->pBuffer=NULL;}
	
	if(m_bActive)
	{
		if(pData->nLocation==-1){pData->nLocation=glGetUniformLocationARB(m_hShaderProgram, sUniformName.c_str());}
		if(pData->nLocation!=-1){pData->bModified=false;glUniform1fARB(pData->nLocation,pData->data.fFloats[0]);}
	}
}
  
void COpenGLShader::AddUniform( std::string sUniformName,float fValue1,float fValue2,bool bTemporal)
{
	std::map<std::string,SUniformData*>::iterator i;
	i=m_mUniforms.find(sUniformName);
	if(i==m_mUniforms.end())
	{
		SUniformData *pData=new SUniformData;
		i=m_mUniforms.insert(std::pair<std::string,SUniformData*>(sUniformName,pData)).first;
	}
	else
	{
		SUniformData *pData=i->second;
		if(pData->eType==eUniformType_Vector2 && (float)pData->data.fFloats[0]==fValue1 && (float)pData->data.fFloats[1]==fValue2)
		{
			// Not mofidied
			return;
		}
	}	  
	SUniformData *pData=i->second;
	pData->eType=eUniformType_Vector2;
	pData->data.fFloats[0]=fValue1;
	pData->data.fFloats[1]=fValue2;
	pData->bTemporal=bTemporal;
	pData->bModified=!bTemporal;
	pData->nElements=0;
	if(pData->pBuffer){delete [] pData->pBuffer;pData->pBuffer=NULL;}
	
	if(m_bActive)
	{
		if(pData->nLocation==-1){pData->nLocation=glGetUniformLocationARB(m_hShaderProgram, sUniformName.c_str());}
		if(pData->nLocation!=-1){pData->bModified=false;glUniform2fvARB(pData->nLocation,1,pData->data.fFloats);}
	}
}
  
void COpenGLShader::AddUniform( std::string sUniformName,const CVector &vVector,bool bTemporal )
{
	std::map<std::string,SUniformData*>::iterator i;
	i=m_mUniforms.find(sUniformName);
	if(i==m_mUniforms.end())
	{
		SUniformData *pData=new SUniformData;
		i=m_mUniforms.insert(std::pair<std::string,SUniformData*>(sUniformName,pData)).first;
	}
	else
	{
		SUniformData *pData=i->second;
		if(pData->eType==eUniformType_Vector3 && (float)pData->data.fFloats[0]==vVector.c[0] && pData->data.fFloats[1]==(float)vVector.c[1] && (float)pData->data.fFloats[2]==vVector.c[2])
		{
			// Not mofidied
			return;
		}
	}	  
	
	SUniformData *pData=i->second;
	pData->eType=eUniformType_Vector3;
	pData->data.fFloats[0]=(float)vVector.c[0];
	pData->data.fFloats[1]=(float)vVector.c[1];
	pData->data.fFloats[2]=(float)vVector.c[2];
	pData->bTemporal=bTemporal;
	pData->bModified=!bTemporal;
	pData->nElements=0;
	if(pData->pBuffer){delete [] pData->pBuffer;pData->pBuffer=NULL;}
	
	if(m_bActive)
	{
		if(pData->nLocation==-1){pData->nLocation=glGetUniformLocationARB(m_hShaderProgram, sUniformName.c_str());}
		if(pData->nLocation!=-1){pData->bModified=false;glUniform3fvARB(pData->nLocation,1,pData->data.fFloats);}
	}
}
  
void COpenGLShader::AddUniform( std::string sUniformName,const CVector &vColor, float fAlpha ,bool bTemporal)
{
	std::map<std::string,SUniformData*>::iterator i;
	i=m_mUniforms.find(sUniformName);
	if(i==m_mUniforms.end())
	{
		SUniformData *pData=new SUniformData;
		i=m_mUniforms.insert(std::pair<std::string,SUniformData*>(sUniformName,pData)).first;
	}
	else
	{
		SUniformData *pData=i->second;
		if(pData->eType==eUniformType_Color && pData->data.fFloats[0]==(float)vColor.c[0] && pData->data.fFloats[1]==(float)vColor.c[1] && pData->data.fFloats[2]==(float)vColor.c[2] && pData->data.fFloats[3]==fAlpha)
		{
			// Not mofidied
			return;
		}
	}	  
	SUniformData *pData=i->second;
	pData->eType=eUniformType_Color;
	pData->data.fFloats[0]=(float)vColor.c[0];
	pData->data.fFloats[1]=(float)vColor.c[1];
	pData->data.fFloats[2]=(float)vColor.c[2];
	pData->data.fFloats[3]=fAlpha;
	pData->bTemporal=bTemporal;
	pData->bModified=!bTemporal;
	pData->nElements=0;
	if(pData->pBuffer){delete [] pData->pBuffer;pData->pBuffer=NULL;}
	
	if(m_bActive)
	{
		if(pData->nLocation==-1){pData->nLocation=glGetUniformLocationARB(m_hShaderProgram, sUniformName.c_str());}
		if(pData->nLocation!=-1){pData->bModified=false;glUniform4fvARB(pData->nLocation,1,pData->data.fFloats);}
	}
}
  
void COpenGLShader::AddUniform( std::string sUniformName,CMatrix &matrix,bool bTemporal)
{
	std::map<std::string,SUniformData*>::iterator i;
	i=m_mUniforms.find(sUniformName);
	if(i==m_mUniforms.end())
	{
		SUniformData *pData=new SUniformData;
		i=m_mUniforms.insert(std::pair<std::string,SUniformData*>(sUniformName,pData)).first;
	}
	
	SUniformData *pData=i->second;
	pData->eType=eUniformType_Matrix;
	pData->bTemporal=bTemporal;
	pData->bModified=!bTemporal;
	pData->nElements=0;
	if(pData->pBuffer){delete [] pData->pBuffer;pData->pBuffer=NULL;}
	
	for(int x=0;x<16;x++)
	{
		pData->data.fFloats[x]=(float)((double*)matrix.e)[x];
	}
	
	if(m_bActive)
	{
		if(pData->nLocation==-1){pData->nLocation=glGetUniformLocationARB(m_hShaderProgram, sUniformName.c_str());}
		if(pData->nLocation!=-1){pData->bModified=false;glUniformMatrix4fvARB(pData->nLocation,1,true,pData->data.fFloats);}
	}
}  

void COpenGLShader::AddUniformIntegers( std::string sUniformName,unsigned int nValues,int *pValues,bool bTemporal)
{
	std::map<std::string,SUniformData*>::iterator i;
	i=m_mUniforms.find(sUniformName);
	if(i==m_mUniforms.end())
	{
		SUniformData *pData=new SUniformData;
		i=m_mUniforms.insert(std::pair<std::string,SUniformData*>(sUniformName,pData)).first;
	}
	
	SUniformData *pData=i->second;
	
	if(!bTemporal)
	{
		// Do not perform unnecesary reallocs
		if(pData->eType!=eUniformType_IntArray || pData->nElements!=nValues)
		{
			delete [] pData->pBuffer;
			pData->pBuffer=(char*)new int [nValues];
		}
		memcpy(pData->pBuffer,pValues,sizeof(int)*nValues);
		pData->nElements=nValues;
	}
		
	pData->eType=eUniformType_IntArray;
	pData->bTemporal=bTemporal;
	pData->bModified=!bTemporal; // temporal values are only set if the shader is currently active
	
	if(m_bActive)
	{
		if(pData->nLocation==-1){pData->nLocation=glGetUniformLocationARB(m_hShaderProgram, sUniformName.c_str());}
		if(pData->nLocation!=-1){pData->bModified=false;glUniform1ivARB(pData->nLocation,nValues,pValues);}
	}	
}

void COpenGLShader::AddUniformFloats( std::string sUniformName,unsigned int nValues,float *pValues,bool bTemporal)
{
	std::map<std::string,SUniformData*>::iterator i;
	i=m_mUniforms.find(sUniformName);
	if(i==m_mUniforms.end())
	{
		SUniformData *pData=new SUniformData;
		i=m_mUniforms.insert(std::pair<std::string,SUniformData*>(sUniformName,pData)).first;
	}
	
	SUniformData *pData=i->second;
	
	if(!bTemporal)
	{
		// Do not perform unnecesary reallocs
		if(pData->eType!=eUniformType_FloatArray || pData->nElements!=nValues)
		{
			delete [] pData->pBuffer;
			pData->pBuffer=(char*)new float [nValues];
		}
		memcpy(pData->pBuffer,pValues,sizeof(float)*nValues);
		pData->nElements=nValues;
	}
	
	pData->eType=eUniformType_FloatArray;
	pData->bTemporal=bTemporal;
	pData->bModified=!bTemporal; // temporal values are only set if the shader is currently active
	
	if(m_bActive)
	{
		if(pData->nLocation==-1){pData->nLocation=glGetUniformLocationARB(m_hShaderProgram, sUniformName.c_str());}
		if(pData->nLocation!=-1){pData->bModified=false;glUniform1fvARB(pData->nLocation,nValues,pValues);}
	}	
}

void COpenGLShader::AddUniformVectors( std::string sUniformName,unsigned int nValues,const CVector *pvVectors,bool bTemporal)
{
	std::map<std::string,SUniformData*>::iterator i;
	i=m_mUniforms.find(sUniformName);
	if(i==m_mUniforms.end())
	{
		SUniformData *pData=new SUniformData;
		i=m_mUniforms.insert(std::pair<std::string,SUniformData*>(sUniformName,pData)).first;
	}
	
	SUniformData *pData=i->second;
	if(pData->eType!=eUniformType_Vector3Array|| pData->nElements!=nValues)
	{
		delete [] pData->pBuffer;
		pData->pBuffer=(char*)new float [nValues*3];
	}
	
	pData->eType=eUniformType_Vector3Array;
	pData->bTemporal=bTemporal;
	pData->bModified=!bTemporal;
	pData->nElements=nValues;
	
	float *pFloats=(float*)pData->pBuffer;
	for(int x=0;x<nValues;x++)
	{
		*pFloats++=(float)pvVectors->c[0];
		*pFloats++=(float)pvVectors->c[1];
		*pFloats++=(float)pvVectors->c[2];
		pvVectors++;
	}
	
	if(m_bActive)
	{
		if(pData->nLocation==-1){pData->nLocation=glGetUniformLocationARB(m_hShaderProgram, sUniformName.c_str());}
		if(pData->nLocation!=-1){pData->bModified=false;glUniform3fvARB(pData->nLocation,nValues,(float*)pData->pBuffer);}
	}
}

void COpenGLShader::AddUniformColors( std::string sUniformName,unsigned int nValues,const CVector *pvColors, float *pvfAlphas,bool bTemporal)
{
	std::map<std::string,SUniformData*>::iterator i;
	i=m_mUniforms.find(sUniformName);
	if(i==m_mUniforms.end())
	{
		SUniformData *pData=new SUniformData;
		i=m_mUniforms.insert(std::pair<std::string,SUniformData*>(sUniformName,pData)).first;
	}
	
	SUniformData *pData=i->second;
	if(pData->eType!=eUniformType_ColorArray|| pData->nElements!=nValues)
	{
		delete [] pData->pBuffer;
		pData->pBuffer=(char*)new float [nValues*4];
	}
	
	pData->eType=eUniformType_ColorArray;
	pData->bTemporal=bTemporal;
	pData->bModified=!bTemporal;
	pData->nElements=nValues;
	
	float *pFloats=(float*)pData->pBuffer;
	for(int x=0;x<nValues;x++)
	{
		*pFloats++=(float)pvColors->c[0];
		*pFloats++=(float)pvColors->c[1];
		*pFloats++=(float)pvColors->c[2];
		*pFloats++=*pvfAlphas;
		pvfAlphas++;
		pvColors++;
	}
	
	if(m_bActive)
	{
		if(pData->nLocation==-1){pData->nLocation=glGetUniformLocationARB(m_hShaderProgram, sUniformName.c_str());}
		if(pData->nLocation!=-1){pData->bModified=false;glUniform4fvARB(pData->nLocation,nValues,(float*)pData->pBuffer);}
	}
}

void COpenGLShader::AddUniformMatrixes( std::string sUniformName,unsigned int nValues,double *pMatrixes,bool bTemporal)
{
	std::map<std::string,SUniformData*>::iterator i;
	i=m_mUniforms.find(sUniformName);
	if(i==m_mUniforms.end())
	{
		SUniformData *pData=new SUniformData;
		i=m_mUniforms.insert(std::pair<std::string,SUniformData*>(sUniformName,pData)).first;
	}
	
	SUniformData *pData=i->second;
	if(pData->eType!=eUniformType_MatrixArray|| pData->nElements!=nValues)
	{
		delete [] pData->pBuffer;
		pData->pBuffer=(char*)new float [nValues*16];
	}
	
	pData->eType=eUniformType_MatrixArray;
	pData->bTemporal=bTemporal;
	pData->bModified=!bTemporal;
	pData->nElements=nValues;
	
	float *pFloats=(float*)pData->pBuffer;
	for(int x=0;x<nValues*16;x++){pFloats[x]=(float)pMatrixes[x];}
	
	if(m_bActive)
	{
		if(pData->nLocation==-1){pData->nLocation=glGetUniformLocationARB(m_hShaderProgram, sUniformName.c_str());}
		if(pData->nLocation!=-1){pData->bModified=false;glUniformMatrix4fvARB(pData->nLocation,nValues,true,(float*)pData->pBuffer);}
	}
}  

void COpenGLShader::AddUniformMatrixes( std::string sUniformName,unsigned int nValues,float *pMatrixes,bool bTemporal)
{
	std::map<std::string,SUniformData*>::iterator i;
	i=m_mUniforms.find(sUniformName);
	if(i==m_mUniforms.end())
	{
		SUniformData *pData=new SUniformData;
		i=m_mUniforms.insert(std::pair<std::string,SUniformData*>(sUniformName,pData)).first;
	}
	
	SUniformData *pData=i->second;
	
	if(!bTemporal)
	{
		// Do not perform unnecesary reallocs
		if(pData->eType!=eUniformType_MatrixArray || pData->nElements!=nValues)
		{
			delete [] pData->pBuffer;
			pData->pBuffer=(char*)new float [nValues*16];
		}
		memcpy(pData->pBuffer,pMatrixes,sizeof(float)*nValues*16);
		pData->nElements=nValues;
	}
	else
	{
		// Remove previous data, this is now a temrporal uniform
		if(pData->pBuffer){delete [] pData->pBuffer;pData->pBuffer=NULL;}
		pData->nElements=0;
		pData->pBuffer=NULL;
	}
	
	pData->eType=eUniformType_MatrixArray;
	pData->bTemporal=bTemporal;
	pData->bModified=!bTemporal; // temporal values are only set if the shader is currently active
	
	if(m_bActive)
	{
		if(pData->nLocation==-1){pData->nLocation=glGetUniformLocationARB(m_hShaderProgram, sUniformName.c_str());}
		if(pData->nLocation!=-1){pData->bModified=false;glUniformMatrix4fvARB(pData->nLocation,nValues,true,pMatrixes);}
	}	
}  


void COpenGLShader::AddAttribute( std::string sAttributeName,int nIndex)
{
	SAttributeData data;
	data.nIndex=nIndex;
	data.bModified=true;
	m_mAttributes[sAttributeName]=data;
}

void COpenGLShader::Deactivate()
{
	if(m_hShaderProgram){glUseProgramObjectARB(0);}
	m_bActive=false;
}

bool COpenGLShader::Unserialize(ISystemPersistencyNode *piNode)
{
	bool bOk=CSystemObjectBase::Unserialize(piNode);
	std::string sVertexShaderCode;
	std::string sFragmentShaderCode;
	if(m_sVertexShader.c_str()){LoadCodeFile(m_sVertexShader,&sVertexShaderCode);}
	if(m_sFragmentShader.c_str()){LoadCodeFile(m_sFragmentShader,&sFragmentShaderCode);}
	m_sVertexShaderCode=m_sPreprocessorDefinitions+"\n"+sVertexShaderCode;
	m_sFragmentShaderCode=m_sPreprocessorDefinitions+"\n"+sFragmentShaderCode;
	m_bTriedToCompile=false;
	return bOk;
}

void COpenGLShader::Load( std::string sVertexShaderFile,std::string sFragmentShaderFile,std::string sPreprocessorDefinitions)
{
	m_sFragmentShader=sFragmentShaderFile;
	m_sFragmentShaderCode="";
	m_sVertexShader=sVertexShaderFile;
	m_sVertexShaderCode="";
	m_sPreprocessorDefinitions=sPreprocessorDefinitions;
	std::string sVertexShaderCode;
	std::string sFragmentShaderCode;
	if(m_sVertexShader.length()){LoadCodeFile(m_sVertexShader,&sVertexShaderCode);}
	if(m_sFragmentShader.length()){LoadCodeFile(m_sFragmentShader,&sFragmentShaderCode);}
	m_sVertexShaderCode=sPreprocessorDefinitions+"\n"+sVertexShaderCode;
	m_sFragmentShaderCode=sPreprocessorDefinitions+"\n"+sFragmentShaderCode;
	m_bTriedToCompile=false;
}

void COpenGLShader::Create( std::string sVertexShaderCode,std::string sFragmentShaderCode,std::string sPreprocessorDefinitions)
{
	m_sFragmentShader="";
	m_sFragmentShaderCode=sPreprocessorDefinitions+"\n"+sFragmentShaderCode;
	m_sVertexShader="";
	m_sVertexShaderCode=sPreprocessorDefinitions+"\n"+sVertexShaderCode;
	m_sPreprocessorDefinitions=sPreprocessorDefinitions;
	m_bTriedToCompile=false;
}
