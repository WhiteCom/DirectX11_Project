#pragma once

#include "CommonHeader.h"

class AnimatedTexture
{
public:
	AnimatedTexture()
	{
	}

	AnimatedTexture(const DirectX::XMFLOAT2& origin, float rotation, float scale, float depth)
	{
		m_Origin = origin;
		m_Rotation = rotation;
		m_Scale = DirectX::XMFLOAT2(scale, scale);
		m_Depth = depth;
	}

	void Load(ID3D11ShaderResourceView* texture, int frameCount, int framesPerSecond)
	{
		if (frameCount < 0 || framesPerSecond <= 0)
			throw std::invalid_argument("AnimatedTexture");

		m_Paused = false;
		m_FrameCount = frameCount;
		m_TimePerFrame = 1.0f / float(framesPerSecond);
		m_TotalElapsed = 0.0f;
		m_Texture = texture;

		if (texture != nullptr)
		{
			Microsoft::WRL::ComPtr<ID3D11Resource> resource;
			texture->GetResource(resource.GetAddressOf());

			D3D11_RESOURCE_DIMENSION dim;
			resource->GetType(&dim);

			if (dim != D3D11_RESOURCE_DIMENSION_TEXTURE2D)
				throw std::exception("AnimatedTexture excepts a Texture2D");

			Microsoft::WRL::ComPtr<ID3D11Texture2D> tex2D;
			resource.As(&tex2D);

			D3D11_TEXTURE2D_DESC desc;
			tex2D->GetDesc(&desc);

			m_TextureWidth = int(desc.Width);
			m_TextureHeight = int(desc.Height);
		}
	}

	void Update(float elapsed)
	{
		if (true == m_Paused)
			return;

		m_TotalElapsed += elapsed;

		if (m_TotalElapsed > m_TimePerFrame)
		{
			++m_Frame;
			m_Frame = m_Frame % m_FrameCount;
			m_TotalElapsed -= m_TimePerFrame;
		}
	}

	void Draw(DirectX::SpriteBatch* batch, const DirectX::XMFLOAT2& screenPos) const
	{
		Draw(batch, m_Frame, screenPos);
	}

	void Draw(DirectX::SpriteBatch* batch, int frame, const DirectX::XMFLOAT2& screenPos) const
	{
		int frameWidth = m_TextureWidth / m_FrameCount;

		RECT sourceRect;
		sourceRect.left = frameWidth * frame;
		sourceRect.top = 0;
		sourceRect.right = sourceRect.left + frameWidth;
		sourceRect.bottom = m_TextureHeight;

		batch->Draw(
			m_Texture.Get(),
			screenPos,
			&sourceRect,
			DirectX::Colors::White,
			m_Rotation,
			m_Origin,
			m_Scale,
			DirectX::SpriteEffects_None,
			m_Depth);
	}

	void Reset()
	{
		m_Frame = 0;
		m_TotalElapsed = 0.0f;
	}

	void Stop()
	{
		m_Paused = true;
		m_Frame = 0;
		m_TotalElapsed = 0.0f;
	}

	void Play() { m_Paused = false; }
	void Paused() { m_Paused = true; }

	bool IsPaused() const { return m_Paused; }

private:
	bool	m_Paused;
	int		m_Frame;
	int		m_FrameCount;
	int		m_TextureWidth;
	int		m_TextureHeight;
	float	m_TimePerFrame;
	float	m_TotalElapsed;
	float	m_Depth;
	float	m_Rotation;

	DirectX::XMFLOAT2	m_Origin;
	DirectX::XMFLOAT2	m_Scale;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	m_Texture;
};