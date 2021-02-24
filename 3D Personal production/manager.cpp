//=============================================================================
//
// マネージャー処理 [manager.cpp]
// Author : 
//
//=============================================================================

//=============================================================================
//インクルードファイル
//=============================================================================
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "keyboard.h"
#include "scene3D.h"
#include "model.h"
#include "game.h"
#include "fade.h"
#include "player.h"
#include "bg.h"

//=============================================================================
//静的メンバ変数宣言
//=============================================================================
CManager::MODE_TYPE CManager::m_mode = CManager::MODE_TYPE_GAME;
CRenderer *CManager::m_pRenderer = NULL;	//レンダラークラスのポインタ変数
CInputKeyboard *CManager::m_pInput = NULL;//入力処理クラスのポインタ変数
CFade *CManager::m_pFade = NULL;
CGame *CManager::m_pGame = NULL;
CScene *CManager::m_pScene = NULL;
//=============================================================================
//コンストラクタ
//=============================================================================
CManager::CManager()
{
}

//=============================================================================
//デストラクタ
//=============================================================================
CManager::~CManager()
{
}

//=============================================================================
//初期化処理
//=============================================================================
HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, bool bWindow)
{
	//レンダラークラスのインスタンス生成
	m_pRenderer = new CRenderer;

	//メモリが確保できたら
	if (m_pRenderer != NULL)
	{
		// 初期化処理
		if (FAILED(m_pRenderer->Init(hWnd, bWindow)))
		{
			return -1;
		}
	}

	//入力処理クラスのインスタンス生成
	m_pInput = new CInputKeyboard;

	//メモリが確保できたら
	if (m_pInput != NULL)
	{
		if (FAILED(m_pInput->Init(hInstance, hWnd)))
		{
			return -1;
		}
	}

	//タイトルクラスのクリエイト
	//m_pTitle = CTitle::Create();

	//フェードクラスのクリエイト
	m_pFade = CFade::Create();

	//全テクスチャの読み込み
	LoadAll();

	return S_OK;
}

//=============================================================================
//終了処理
//=============================================================================
void CManager::Uninit(void)
{
	//全テクスチャの破棄
	UnLoadAll();

	if (m_pFade != NULL)
	{
		//フェードクラスの終了処理呼び出し
		m_pFade->Uninit();

		//メモリの破棄
		delete m_pFade;

		//メモリのクリア
		m_pFade = NULL;
	}

	if (m_pInput != NULL)
	{
		//入力処理クラスの終了処理呼び出し
		m_pInput->Uninit();

		//メモリの削除
		delete m_pInput;

		//メモリのクリア
		m_pInput = NULL;
	}

	if (m_pRenderer != NULL)
	{
		//レンダラークラスの終了処理呼び出し
		m_pRenderer->Uninit();

		//メモリの削除
		delete m_pRenderer;

		//メモリのクリア
		m_pRenderer = NULL;
	}
}

//=============================================================================
//更新処理
//=============================================================================
void CManager::Update(void)
{
	if (m_pInput != NULL)
	{
		//入力処理クラスの更新処理呼び出し
		m_pInput->Update();
	}

	if (m_pRenderer != NULL)
	{
		//レンダラークラスの更新処理呼び出し
		m_pRenderer->Update();
	}

	if (m_pFade != NULL)
	{
		//フェードクラスの更新処理呼び出し
		m_pFade->Update();
	}

	/*switch (m_mode)
	{
	case MODE_TYPE_GAME:
	if (CManager::GetJoypad()->GetJoystickTrigger(CInputJoypad::JOY_BUTTON_START, 0))
	{
	m_pScene->GetPause(false);
	}

	else if (CManager::GetJoypad()->GetJoystickTrigger(CInputJoypad::JOY_BUTTON_B, 0))
	{
	m_pScene->GetPause(true);
	}
	break;
	}*/
}

//=============================================================================
//描画処理
//=============================================================================
void CManager::Draw(void)
{
	if (m_pRenderer != NULL)
	{
		//レンダラークラスの描画処理呼び出し
		m_pRenderer->Draw();
	}
}

//=============================================================================
//テクスチャの全ロード処理
//=============================================================================
void CManager::LoadAll(void)
{
	CBg::Load();
}

//=============================================================================
//テクスチャの全アンロード処理
//=============================================================================
void CManager::UnLoadAll(void)
{
	CBg::UnLoad();
}

//=============================================================================
//ゲームモードの設定処理
//=============================================================================
void CManager::SetMode(MODE_TYPE mode)
{

	//現在モードの終了
	switch (m_mode)
	{
	
	case MODE_TYPE_GAME:
		if (m_pGame != NULL)
		{
			m_pGame->Uninit();
			m_pGame = NULL;
		}
		break;
	default:
		break;
	}

	CScene::ReleaseAll();

	//モードを設定
	m_mode = mode;

	//設定されたモードをクリエイト
	switch (m_mode)
	{
	case MODE_TYPE_GAME:
		if (m_pGame == NULL)
		{
			m_pGame = CGame::Create();
		}

		break;
	default:
		break;
	}
}

//=============================================================================
//ゲームモード情報の取得
//=============================================================================
CManager::MODE_TYPE CManager::GetMode(void)
{
	return m_mode;
}

//=============================================================================
//レンダラー情報取得
//=============================================================================
CRenderer * CManager::GetRenderer(void)
{
	return m_pRenderer;
}


//=============================================================================
//キーボード情報取得
//=============================================================================
CInputKeyboard * CManager::GetKeyboard(void)
{
	return m_pInput;
}

//=============================================================================
//フェード情報取得
//=============================================================================
CFade * CManager::GetFade(void)
{
	return m_pFade;
}