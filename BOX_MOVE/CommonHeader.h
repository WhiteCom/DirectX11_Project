#pragma once

//
// Win32 Header
//
#include <windows.h>
#include <sdkddkver.h>

//
// Cpp Header
//
#include <algorithm>
#include <vector>
#include <exception>
#include <memory>
#include <stdexcept>

//
// DirectX 11 Header
// ( DirectXTK Lib )
//
#include <wrl/client.h>

#include <d3d11_1.h>
#include <dxgi1_2.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <directxcolors.h>

#include "CommonStates.h"
#include "DDSTextureLoader.h"
#include "DirectXHelpers.h"
#include "Effects.h"
#include "GamePad.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "PrimitiveBatch.h"
#include "ScreenGrab.h"
#include "SimpleMath.h"
#include "SpriteBatch.h"
#include "SpriteFont.h"
#include "VertexTypes.h"
#include "WICTextureLoader.h"
