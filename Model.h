#pragma once
#include "objfilemodel.h"
#include "LightManager.h"
#include <list>

//General buffer to be sent to the shader - 208 bytes
struct MODEL_CONSTANT_BUFFER
{
	XMMATRIX WorldViewProjection;	//64 bytes
	XMVECTOR camPos;				//16 bytes
	XMVECTOR dirLightCol;			//16 bytes
	XMVECTOR dirLightPos;			//16 bytes
	XMVECTOR ambLightCol;			//16 bytes

	XMVECTOR pointLightPos;			//16 bytes
	XMVECTOR pointLightCol;			//16 bytes
	float pointLightRange;			//4 bytes

	XMVECTOR spotLightPos;			//16 bytes
	XMVECTOR spotLightDir;			//16 bytes
	XMVECTOR spotLightCol;			//16 bytes
	float spotLightRange;			//4 bytes
	float spotLightInnerCone;		//4 bytes
	float spotLightOuterCone;		//4 bytes
};

//Shiny specific buffer
struct SHINYMODEL_CONSTANT_BUFFER
{
	XMMATRIX WorldView; // 64 bytes
};

//Dissolve specific buffer
struct DISSOLVE_CONSTANT_BUFFER
{
	float dissolveAmount; // 4 bytes
	float specularIntensity; //4 bytes
	float specExp; //4bytes
	float fill; //4 bytes

};

enum ModelType{Normal, Shiny, Dissolve};
enum CollisionType{Sphere, Box, Mesh};
class Model
{
private:
	ID3D11Device*				m_pD3DDevice;
	ID3D11DeviceContext*		m_pImmediateContext;

	ObjFileModel*				m_pObject;
	ID3D11VertexShader*			m_pVShader;
	ID3D11PixelShader*			m_pPShader;
	ID3D11InputLayout*			m_pInputLayout;
	ID3D11Buffer*				m_pConstantBuffer;
	ID3D11Buffer*				m_pShinyBuffer;
	ID3D11Buffer*				m_pDissolveBuffer;

	ID3D11ShaderResourceView*	m_pTexture;
	ID3D11SamplerState*			m_pSampler;

	LightManager*				m_lights;

	float						m_boundingSphereCentreX, m_boundingSphereCentreY, m_boundingSphereCentreZ, m_boundingSphereRadius, m_defRadius;
	xyz							m_boundingBoxCentre, m_boundingBoxSize;
	void						CalculateModelCentrePoint();
	void						CalculateBoudingSphereRadius();

	void						CalculateBoundingBox();
	
	ModelType					m_type;
	CollisionType				m_collisionType;

	//Used for a shiny model
	ID3D11ShaderResourceView*	m_pSkyboxTexture;

	//Used for a dissolve model
	ID3D11ShaderResourceView*	m_pDissolveTexture;
	ID3D11BlendState*			m_pTransparencyBlend;
	ID3D11SamplerState*			m_pAlphaSampler;

	float						m_dissolveAmount;
public:
	Model(ID3D11Device* device, ID3D11DeviceContext* deviceContext, LightManager* lights);
	~Model();
	HRESULT LoadObjModel(char* filename);
	void Draw(XMMATRIX* world, XMMATRIX* view, XMMATRIX* projection, xyz camPos);

	//Texture creation
	void SetTexture(ID3D11ShaderResourceView* texture)					{ m_pTexture = texture; }
	void SetSampler(ID3D11SamplerState* sampler)						{ m_pSampler = sampler; }

	//Used for dissolve shaders;
	void SetDissolveTexture(ID3D11ShaderResourceView* texture)			{ m_pDissolveTexture = texture; }
	void SetDissolveAmount(float val);
	float GetDissolveAmount(void)										{ return m_dissolveAmount; }

	//Custom Shader
	HRESULT LoadDefaultShaders(void);
	HRESULT LoadCustomShader(char* fileName, char* vertexShaderFunction, char* pixelShaderFunction);


	float GetBoundingSphereRadius(void);


	float GetBoundingSphereX(void)										{ return m_boundingSphereCentreX; }
	float GetBoundingSphereY(void)										{ return m_boundingSphereCentreY; }
	float GetBoundingSphereZ(void)										{ return m_boundingSphereCentreZ; }

	void ChangeModelType(ModelType newType)								{ m_type = newType; }
	ObjFileModel* GetModel(void)										{ return m_pObject; }

	void SetCollisionType(CollisionType newType);

	CollisionType GetCollisionType(void)								{ return m_collisionType; }

	xyz GetBoundingBoxSize(void)										{ return m_boundingBoxSize; }
	xyz GetBoundingBoxCentre(void)										{ return m_boundingBoxCentre; }

	//Used for shiny model
	void SetSkyboxTexture(ID3D11ShaderResourceView* skybox)				{ m_pSkyboxTexture = skybox; }
};