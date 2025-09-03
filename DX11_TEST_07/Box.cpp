#include "Box.h"
#include "CommonBindable.h"

Box::Box(Graphics& graphics,
	std::mt19937& rng,
	std::uniform_real_distribution<float>& adist,
	std::uniform_real_distribution<float>& ddist,
	std::uniform_real_distribution<float>& odist,
	std::uniform_real_distribution<float>& rdist)
	: 
	m_r(rdist(rng)),
	m_droll(ddist(rng)),
	m_dpitch(ddist(rng)),
	m_dyaw(ddist(rng)),
	m_dphi(odist(rng)),
	m_dtheta(odist(rng)),
	m_dchi(odist(rng)),
	m_chi(adist(rng)),
	m_theta(adist(rng)),
	m_phi(adist(rng))
{
	if (!IsStaticInitialized())
	{
		struct Vertex
		{
			struct Position
			{
				float x, y, z;
			};

			Position pos;
		};

		const std::vector<Vertex> vertices =
		{
			{ -1.0f, -1.0f, -1.0f },
			{  1.0f, -1.0f, -1.0f },
			{ -1.0f,  1.0f, -1.0f },
			{  1.0f,  1.0f, -1.0f },
			{ -1.0f, -1.0f,  1.0f },
			{  1.0f, -1.0f,  1.0f },
			{ -1.0f,  1.0f,  1.0f },
			{  1.0f,  1.0f,  1.0f },
		};

		AddStaticBind(std::make_unique<VertexBuffer>(graphics, vertices));

		auto pVertexShader = std::make_unique<VertexShader>(graphics, L"VertexShader.cso");
		//std::unique_ptr<VertexShader> pVertexShader = std::make_unique<VertexShader>(graphics, L"VertexShader.cso");
		auto pVertexShaderByteCode = pVertexShader->GetByteCode();

		AddStaticBind( std::move(pVertexShader) );

		AddStaticBind( std::make_unique<PixelShader>(graphics, L"PixelShader.cso") );

		const std::vector<unsigned short> indices =
		{
			0,2,1, 2,3,1,
			1,3,5, 3,7,5,
			2,6,3, 3,6,7,
			4,5,7, 4,7,6,
			0,4,2, 2,4,6,
			0,1,4, 1,5,4,
		};

		AddStaticIndexBuffer( std::make_unique<IndexBuffer>(graphics, indices) );

		struct ConstantBuffer2
		{
			struct Colors
			{
				float r, g, b, a;
			};

			Colors face_colors[6];
		};

		const ConstantBuffer2 cb2 =
		{
			{
				{ 1.0f, 0.0f, 1.0f },
				{ 1.0f, 0.0f, 0.0f },
				{ 0.0f, 1.0f, 0.0f },
				{ 0.0f, 0.0f, 1.0f },
				{ 1.0f, 1.0f, 0.0f },
				{ 0.0f, 1.0f, 1.0f },
			}
		};

		AddStaticBind(std::make_unique< PixelConstantBuffer< ConstantBuffer2>>(graphics, cb2));

		const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
		{
			{"Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		};

		AddStaticBind(std::make_unique<InputLayout>(graphics, ied, pVertexShaderByteCode));

		AddStaticBind(std::make_unique<Topology>(graphics, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
	}
	else
	{
		SetIndexFromStatic();
	}

	AddBind(std::make_unique<TransformCBuf>(graphics, *this));
}

void Box::Update(float dt) noexcept
{
	m_roll += m_droll * dt;
	m_pitch += m_dpitch * dt;
	m_yaw += m_dyaw * dt;
	m_theta += m_dtheta * dt;
	m_phi += m_dphi * dt;
	m_chi += m_dchi * dt;
}

DirectX::XMMATRIX Box::GetTransformXM() const noexcept
{
	return DirectX::XMMatrixRotationRollPitchYaw(m_pitch, m_yaw, m_roll)
		* DirectX::XMMatrixTranslation(m_r, 0.0f, 0.0f)
		* DirectX::XMMatrixRotationRollPitchYaw(m_theta, m_phi, m_chi)
		* DirectX::XMMatrixTranslation(0.0f, 0.0f, 20.0f);
}