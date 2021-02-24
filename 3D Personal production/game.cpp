//=======================================================================================
//
// ゲーム処理 [game.cpp]
// Author : Konishi Yuuto
//
//=======================================================================================

//=======================================================================================
// インクルード
//=======================================================================================
#include "game.h"
#include "camera.h"
#include "light.h"
#include "model.h"
#include "manager.h"
#include "renderer.h"
#include "player.h"
#include "bg.h"
#include "time.h"
#include "keyboard.h"


//=======================================================================================
// static初期化
//=======================================================================================
CCamera *CGame::m_apCamera[MAX_PLAYER] = {};		// カメラクラスのポインタ変数
CLight *CGame::m_pLight = NULL;						// ライトクラスのポインタ変数
CMeshField *CGame::m_pMeshField = NULL;				// メッシュフィールド
CMeshShape *CGame::m_pSphere = NULL;				// メッシュスフィア
CBg *CGame::m_pBg = NULL;							// 背景のポインタ
CPlayer *CGame::m_apPlayer[MAX_PLAYER] = {};		// プレイヤーのポインタ
CTime *CGame::m_pTime = NULL;						// タイムのポインタ
CUi *CGame::m_pUi = NULL;							// uiのポインタ
CUiStart *CGame::m_pUiStart = NULL;
CLockon *CGame::m_pLockon = NULL;
CBill *CGame::m_pBill = NULL;
CContinue *CGame::m_pContinue = NULL;
CUiEnd *CGame::m_pUiEnd = NULL;
CPause *CGame::m_pPause = NULL;
int CGame::m_nRoundNum = 0;
int CGame::m_aWinNum[MAX_PLAYER] = {};
CSea *CGame::m_pSea = NULL;

//=======================================================================================
// コンストラクタ
//=======================================================================================
CGame::CGame(int nPriority) : CScene(nPriority)
{
	m_bGameEnd = false;
}

//=======================================================================================
// デストラクタ
//=======================================================================================
CGame::~CGame()
{
	m_nRoundNum = 0;
}

//=======================================================================================
// クリエイト
//=======================================================================================
CGame* CGame::Create(void)
{
	// メモリ確保
	CGame* pGame = new CGame();

	// 初期化処理
	pGame->Init(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	return pGame;
}

//=======================================================================================
// 初期化処理
//=======================================================================================
HRESULT CGame::Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 size)
{
	m_nRoundNum = 0;

	CInputKeyboard *pKeyboard = CManager::GetKeyboard();

	for (int nCount = 0; nCount < MAX_PLAYER; nCount++)
	{
		//カメラクラスのクリエイト
		m_apCamera[nCount] = CCamera::Create();
	}

	//ライトクラスの生成
	m_pLight = new CLight;

	// ライトの初期化処理
	if (m_pLight != NULL)
	{
		if (FAILED(m_pLight->Init()))
		{
			return -1;
		}
	}

	// プレイヤーの生成
	if (m_apPlayer[0] == NULL)
	{
		m_apPlayer[0] = CPlayer::Create(D3DXVECTOR3(0.0f, 0.0f, -1000.0f), D3DXVECTOR3(PLAYER_SIZE_X, PLAYER_SIZE_Y, PLAYER_SIZE_Z));
	}
	if (m_apPlayer[1] == NULL)
	{
		m_apPlayer[1] = CPlayer::Create(D3DXVECTOR3(0.0f, 0.0f, 1000.0f), D3DXVECTOR3(PLAYER_SIZE_X, PLAYER_SIZE_Y, PLAYER_SIZE_Z));
	}

	// 背景
	if (m_pBg == NULL)
	{
		m_pBg = CBg::Create();
	}

	//デバイス情報の取得
	LPDIRECT3DDEVICE9 pD3DDevice = CManager::GetRenderer()->GetDevice();

	//フォントの生成
	D3DXCreateFont(pD3DDevice, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Terminal", &m_pFont);

	return S_OK;
}

//=======================================================================================
// 終了処理
//=======================================================================================
void CGame::Uninit(void)
{
	for (int nCount = 0; nCount < MAX_PLAYER; nCount++)
	{
		if (m_apCamera[nCount] != NULL)
		{
			//カメラクラスの終了処理呼び出す
			m_apCamera[nCount]->Uninit();

			//メモリの破棄
			delete m_apCamera[nCount];

			//メモリのクリア
			m_apCamera[nCount] = NULL;
		}
	}

	//CMeshShape::UnLoad();
	//// メッシュスフィア
	//if (m_pSphere != NULL)
	//{
	//	m_pSphere->Uninit();
	//}

	// 背景
	if (m_pBg != NULL)
	{
		m_pBg->Uninit();
		m_pBg = NULL;
	}

	// ライトの終了処理
	if (m_pLight != NULL)
	{
		m_pLight->Uninit();
		delete m_pLight;
		m_pLight = NULL;
	}

	// プレイヤー終了処理
	for (int nCount = 0; nCount < MAX_PLAYER; nCount++)
	{
		if (m_apPlayer[nCount] != NULL)
		{
			m_apPlayer[nCount]->Uninit();
			m_apPlayer[nCount] = NULL;
		}
	}
}

//=======================================================================================
// 更新処理
//=======================================================================================
void CGame::Update(void)
{
	for (int nCount = 0; nCount < MAX_PLAYER; nCount++)
	{
		if (m_apCamera[nCount] != NULL)
		{
			//カメラクラスの更新処理

			m_apCamera[nCount]->Update();
		}
	}
}

//=======================================================================================
// 描画処理
//=======================================================================================
void CGame::Draw(void)
{
	// 背景
	if (m_pBg != NULL)
	{
		m_pBg->Draw();
	}
}

//=======================================================================================
// カメラの情報
//=======================================================================================
CCamera * CGame::GetCamera(int nCount)
{
	return m_apCamera[nCount];
}

//=======================================================================================
// ライトの情報
//=======================================================================================
CLight * CGame::GetLight(void)
{
	return m_pLight;
}

//=======================================================================================
// プレイヤーの情報
//=======================================================================================
CPlayer * CGame::GetPlayer(int nCount)
{
	return m_apPlayer[nCount];
}

//=======================================================================================
// タイムの情報
//=======================================================================================
CTime * CGame::GetTime(void)
{
	return m_pTime;
}

//=======================================================================================
// uiの情報
//=======================================================================================
CUi * CGame::GetUi(void)
{
	return m_pUi;
}


//=======================================================================================
// uistartの情報
//=======================================================================================
CUiStart * CGame::GetUiStart(void)
{
	return m_pUiStart;
}

//=======================================================================================
// uiendの情報
//=======================================================================================
CUiEnd *CGame::GetUiEnd(void)
{
	return m_pUiEnd;
}

CMeshShape * CGame::GetSphere()
{
	return m_pSphere;
}

//=======================================================================================
// ロックオンの情報
//=======================================================================================
CLockon * CGame::GetLockon(void)
{

	return m_pLockon;
}

//=======================================================================================
// ビルの情報
//=======================================================================================
CBill * CGame::GetBill(void)
{
	return m_pBill;
}

//=======================================================================================
// コンテニューの情報
//=======================================================================================
CContinue * CGame::GetContinue(void)
{
	return m_pContinue;
}

//=======================================================================================
// コンテニューの情報
//=======================================================================================
CPause * CGame::GetPause(void)
{
	return m_pPause;
}
