//=============================================================================
//
// メイン処理 [camera.cpp]
// Author : 高橋那伯志
//
//=============================================================================
#include "camera.h"
#include "manager.h"
#include "input.h"
#include "renderer.h"
#include "keyboard.h"
#include "game.h"
#include "player.h"

//=============================================================================
//マクロ定義
//=============================================================================
#define CAMERA_DEFAULT_Fθ			(D3DXToRadian(75.0f))	//カメラのθDefault値
#define DISTANCE					(1300.0f)				//視点～注視点の距離
#define PLAYER_HEIGHT				(200.0f)				//注視点の高さ
#define PLAYE_ROT_VERTICAL_FRONT	(D3DXToRadian(0.0f))	//プレイヤーの縦軸前
#define WIN_START_ROT_VERTICAL		(D3DXToRadian(180.0f))	//勝利時縦軸回転の開始角
#define WIN_START_ROT_HORIZONTAL	(D3DXToRadian(40.0f))	//勝利時横軸回転の開始角
#define WIN_START_DISTANCE			(1500.0f)				//勝利時の開始距離

#define WIN_END_ROT_VERTICAL		(D3DXToRadian(360.0f+180.0f))	//勝利時縦軸回転の終了角
#define WIN_END_ROT_HORIZONTAL		(D3DXToRadian(100.0f))	//勝利時横軸回転の終了角
#define WIN_END_DISTANCE			(500.0f)				//勝利時の終了距離
#define WIN_ROT_MOVE_SUBTRACTION	(0.2f)					//勝利時の横軸回転角度変化量

#define LOSE_START_ROT_VERTICAL		(D3DXToRadian(180.0f))	//敗北時縦軸回転の開始角
#define LOSE_START_ROT_HORIZONTAL	(D3DXToRadian(60.0f))	//敗北時横軸回転の開始角
#define LOSE_START_DISTANCE			(500.0f)				//敗北時の開始距離
#define LOSE_END_DISTANCE			(1000.0f)				//敗北時の終了距離
#define LOSE_MOVE_SUBTRACTION		(0.2f)					//敗北時の横軸回転角度変化量

#define CAMERA_MIN_Fφ				(D3DXToRadian(10.0f))	//カメラの最小角
#define CAMERA_MAX_Fφ				(D3DXToRadian(170.0f))	//カメラの最大角
#define CAMERA_MIN_HIGHT			(2.0f)					//カメラの最低高度
#define STICK_SENSITIVITY			(50.0f)					//スティック感度
#define STICK_INPUT_CONVERSION		(D3DXToRadian(1.0f))	//スティック入力変化量
#define JUDGMENT_RANGE				(50.0f)					//判定距離

//=============================================================================
// static初期化
//=============================================================================
int CCamera::m_nCameraAll = 0;

//=============================================================================
// クリエイト
//=============================================================================
CCamera * CCamera::Create(void)
{
	// 初期化処理
	CCamera *pCamera = new CCamera;

	pCamera->Init();

	return pCamera;
}

//=============================================================================
//カメラクラスのコンストラクタ
//=============================================================================
CCamera::CCamera()
{
	//各メンバ変数のクリア
	m_posV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// カメラの座標
	m_posVDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// カメラの座標（目的地）
	m_posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 注視点
	m_posRDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 注視点（目的地）
	m_posU = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 上方向ベクトル
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 向き
	m_fDistance = 0.0f;							// 視点～注視点の距離
	m_fMove = 0.0f;								// 移動量
	m_nCameraNum = m_nCameraAll++;				// カメラの番号
	m_state = CAMERASTATE_NONE;					//ステータス初期化
}

//=============================================================================
//カメラクラスのデストラクタ
//=============================================================================
CCamera::~CCamera()
{
}

//=============================================================================
//カメラクラスの初期化処理
//=============================================================================
HRESULT CCamera::Init(void)
{
	// レンダラーの情報を受け取る
	CRenderer *pRenderer = NULL;
	pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	switch (m_nCameraNum)
	{
	case 0:
		m_fMove = 5.0f;
		m_fDistance = DISTANCE;
		m_rot.y = 0.0f;
		m_fθ = CAMERA_DEFAULT_Fθ;
		m_fφ = D3DXToRadian(180.0f);						// 初期値敵のほう向ける
		m_posR = D3DXVECTOR3(0.0f, PLAYER_HEIGHT, 0.0f);	// 注視点設定
		m_posU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);				// 上方向ベクトル
		m_posV.x = m_posR.x + m_fDistance* sinf(m_fθ) * sinf(m_fφ);	//カメラ位置X
		m_posV.y = m_posR.z + m_fDistance* cosf(m_fθ);					//カメラ位置Y
		m_posV.z = m_posR.y + m_fDistance* sinf(m_fθ) * cosf(m_fφ);	//カメラ位置Z
		break;

	case 1:
		m_fMove = 5.0f;
		m_fDistance = DISTANCE;
		m_fθ = CAMERA_DEFAULT_Fθ;
		m_fφ = D3DXToRadian(0.0f);									// 初期値敵のほう向ける
		m_posR = D3DXVECTOR3(0.0f, PLAYER_HEIGHT, 0.0f);			// 注視点設定
		m_posU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);						// 上方向ベクトル
		m_posV.x = m_posR.x + m_fDistance* sinf(m_fθ) * sinf(m_fφ);	//カメラ位置X
		m_posV.y = m_posR.z + m_fDistance* cosf(m_fθ);					//カメラ位置Y
		m_posV.z = m_posR.y + m_fDistance* sinf(m_fθ) * cosf(m_fφ);	//カメラ位置Z
		break;

	default:
		break;
	}

	//状態の初期設定
	m_state = CAMERASTATE_NORMAL;

	return S_OK;
}

//=============================================================================
//カメラクラスの終了処理
//=============================================================================
void CCamera::Uninit(void)
{
	m_nCameraAll = 0;
}

//=============================================================================
//カメラクラスの更新処理
//=============================================================================
void CCamera::Update(void)
{
	static bool bPlayerWin = false;	//プレイヤー勝利フラグ
	static int nWinPlayer = 0;		//Winプレイヤー番号

									//キーボードクラス情報の取得
	CInputKeyboard *pKeyInput = CManager::GetKeyboard();

	D3DXVECTOR3 PlayerPos[MAX_PLAYER];	//プレイヤー位置
	D3DXVECTOR3 PlayerRot[MAX_PLAYER];	//プレイヤー角度
	CPlayer::PLAYER_STATE PlayerState[MAX_PLAYER];
	bool PlayerWin[MAX_PLAYER];

	if (CGame::GetPlayer(m_nCameraNum) != NULL)
	{
		//プレイヤー1位置取得
		PlayerPos[m_nCameraNum] = CGame::GetPlayer(m_nCameraNum)->GetPos();
		//プレイヤー1角度取得
		PlayerRot[m_nCameraNum] = CGame::GetPlayer(m_nCameraNum)->GetRot();

		switch (m_state)
		{
		case CAMERASTATE_NONE:
			break;

		case CAMERASTATE_NORMAL://通常
			NomalUpdate(PlayerPos, PlayerRot);
			break;

		case CAMERASTATE_MAX://STATEの最大数
			break;

		default:
			break;
		}
	}
}

//=============================================================================
//カメラクラスの試合中更新処理
//=============================================================================
void CCamera::NomalUpdate(D3DXVECTOR3 PlayerPos[], D3DXVECTOR3 PlayerRot[])
{

	//キーボードクラス情報の取得
	CInputKeyboard *pKeyInput = CManager::GetKeyboard();

		//視点（カメラ座標）の左旋回
		if (pKeyInput->GetPress(DIK_LEFT))
		{
			m_fφ += STICK_INPUT_CONVERSION;
		}
		//視点（カメラ座標）の右旋回
		if (pKeyInput->GetPress(DIK_RIGHT))
		{
			m_fφ -= STICK_INPUT_CONVERSION;
		}
		//視点（カメラ座標）の上旋回
		if (pKeyInput->GetPress(DIK_UP) && m_fθ >= CAMERA_MIN_Fφ)
		{
			m_fθ -= STICK_INPUT_CONVERSION;
		}
		//視点（カメラ座標）の下旋回
		if (pKeyInput->GetPress(DIK_DOWN) && m_fθ <= CAMERA_MAX_Fφ)
		{
			m_fθ += STICK_INPUT_CONVERSION;
		}

		m_posVDest.x = PlayerPos[m_nCameraNum].x + m_fDistance * sinf(m_fθ) * sinf(m_fφ);	//カメラ位置X設定
		m_posVDest.y = PlayerPos[m_nCameraNum].y + PLAYER_HEIGHT + m_fDistance * cosf(m_fθ);				//カメラ位置Y設定
		m_posVDest.z = PlayerPos[m_nCameraNum].z + m_fDistance * sinf(m_fθ) * cosf(m_fφ);	//カメラ位置Z設定

		m_posRDest = D3DXVECTOR3(PlayerPos[m_nCameraNum].x, PlayerPos[m_nCameraNum].y + PLAYER_HEIGHT, PlayerPos[m_nCameraNum].z);	//注視点設定

																																	//カメラPOSYの下限
		if (m_posVDest.y <= CAMERA_MIN_HIGHT)
		{
			m_posVDest.y = CAMERA_MIN_HIGHT;	//限界値に戻す
		}

		//設定値の反映
		m_posV += (m_posVDest - m_posV)*0.1;
		m_posR += (m_posRDest - m_posR)*0.9;
}

//=============================================================================
//カメラクラスのセット処理
//=============================================================================
void CCamera::SetCamera(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//ビューマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxView);

	//ビューマトリックスの作成
	D3DXMatrixLookAtLH(&m_mtxView,
		&m_posV,
		&m_posR,
		&m_posU);

	//ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &m_mtxView);

	//プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxProjection);

	//プロジェクションマトリックスの作成
	D3DXMatrixPerspectiveFovLH(&m_mtxProjection,
		D3DXToRadian(45.0f),
		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT / 2,
		10.0f,
		100000.0f);

	//プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION,
		&m_mtxProjection);
}

//=============================================================================
//カメラの位置取得処理
//=============================================================================
D3DXVECTOR3 CCamera::GetposV(void)
{
	return m_posV;
}

//=============================================================================
//カメラの角度取得処理
//=============================================================================
D3DXVECTOR3 CCamera::GetposR(void)
{
	return m_posR;
}

//=============================================================================
//カメラのビューマトリックス取得処理
//=============================================================================
D3DXMATRIX CCamera::GetMtxView(void)
{
	return m_mtxView;
}

//=============================================================================
//カメラの縦回転角取得処理
//=============================================================================
float CCamera::Getθ(void)
{
	return m_fθ;
}

//=============================================================================
//カメラの横回転角取得処理
//=============================================================================
float CCamera::Getφ(void)
{
	return m_fφ;
}