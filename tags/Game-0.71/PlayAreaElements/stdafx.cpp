#include "./stdafx.h"

#ifdef WIN32
#pragma comment (lib, "VectorLib.lib")
#pragma comment (lib, "GameRuntime.lib")
#pragma comment (lib, "GameRuntimeLib.lib")
#endif

CSingletonWrapper<IEntityManager> g_EntityManagerWrapper("GameSystem","EntityManager");
CSingletonWrapper<IPlayAreaManager> g_PlayAreaManagerWrapper("GameSystem","PlayAreaManager");
CSingletonWrapper<IFrameManager>  g_FrameManagerSingleton("GameSystem","FrameManager");