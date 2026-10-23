// ptsound.h - DirectSound emulation on SDL2 audio (see ptsound.cpp).
#pragma once

// compat.h renames the game's DirectSoundCreate calls to this in the GL build.
HRESULT WINAPI PtSound_DirectSoundCreate( LPCGUID pcGuidDevice, LPDIRECTSOUND *ppDS, LPUNKNOWN pUnkOuter );
