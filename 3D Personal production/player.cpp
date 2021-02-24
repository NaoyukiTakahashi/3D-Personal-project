//=============================================================================
//
// プレイヤークラス [player.cpp]
// Author : Konishi Yuuto
//
//=============================================================================

//=============================================================================
// インクルード
//=============================================================================
#include "player.h"
#include <stdio.h>
#include "manager.h"
#include "keyboard.h"
#include "renderer.h"
#include "input.h"
#include "camera.h"
#include "game.h"
#include "time.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define PLAYER_SPEED			(20.0f)				// プレイヤーの移動量
#define PLAYER_DUSH				(37.5f)				// プレイヤーのダッシュ
#define PLAYER_DUSH_INTER		(80)				// ダッシュができる長さ
#define DUSH_NONE_TIME			(50)				// ダッシュできない時間
#define PLAYER_JUMP				(10.0f)				// ジャンプの処理
#define GRAVITY_POWAR			(0.15f)				// 重力の強さ
#define PLAYER_FALL				(-13.0f)			// 急降下の処理
#define GROUND_RIMIT			(0.0f)				// 地面の制限
#define PLAYE_ROT_Y_FRONT		(D3DXToRadian(90.0f))	//プレイヤーの縦軸前
#define PLAYE_ROT_Y_LEFT		(D3DXToRadian(180.0f))	//プレイヤーの縦軸左
#define PLAYE_ROT_Y_RIGHT		(D3DXToRadian(0.0f))	//プレイヤーの縦軸右
#define PLAYE_ROT_Y_BUCK		(D3DXToRadian(-90.0f))	//プレイヤーの縦軸後
#define STICK_SENSITIVITY		(50.0f)					//スティック感度
#define STATE_DAMAGE_TIME		(100)				// ダメージ状態のカウント
#define STATE_EXPLOSION_TIME	(30)				// 爆発状態のカウント
#define STATE_EXPLOSION_END		(500)				// 爆発状態の終了フレーム
#define PLAYER_BOMB_INTER		(150)				// ボムの待機フレーム
#define PLAYER_MISSILE			(150)				// ミサイルの待機フレーム
#define PLAYER_LASER_INTER		(300)				// レーザーの待機フレーム

#define TEST_XFAILE_NAME "data/Text/Test.txt"	//LBXのファイルパス

#define LBX_XFAILE_NAME "data/Text/motion_LBX.txt"	//LBXのファイルパス
#define GANDAMU_XFAILE_NAME "data/Text/motion_gandamu.txt"	//ガンダムのファイルパス
//=============================================================================
// グローバル変数宣言
//=============================================================================
MODELFILLE g_modelfile[MAX_MODEL_PARTS];	//モデルパーツ情報.

//=============================================================================
// static初期化
//=============================================================================
int CPlayer::m_nPlayerAll = 0;				// プレイヤーの総数

//=============================================================================
// クリエイト
//=============================================================================
CPlayer * CPlayer::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	// 初期化処理
	CPlayer *pPlayer = new CPlayer;

	// プレイヤーの番号を代入
	pPlayer->m_nPlayerNum = m_nPlayerAll++;

	// 初期化処理
	pPlayer->Init(pos, size);

	return pPlayer;
}

//=============================================================================
// コンストラクタ
//=============================================================================
CPlayer::CPlayer(int nPriority) : CScene(nPriority)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_OldPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nPlayerNum = 0;						// プレイヤーの番号
	m_fAngle = 0.0f;
}

//=============================================================================
// デストラクタ
//=============================================================================
CPlayer::~CPlayer()
{
	m_nPlayerAll = 0;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CPlayer::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_OldPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fAngle = 0.0f;

	//位置の設定
	m_pos = pos;

	//ROTの初期値設定（敵の方向）
	m_rotDest = D3DXVECTOR3(0.0f, D3DXToRadian(0.0f), 0.0f);

	//ファイル読み込み
	if (FAILED(ReadFile()))
	{
		return E_FAIL;
	}

	//オブジェクトタイプの設定
	SetObjType(CScene::OBJTYPE_PLAYER);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CPlayer::Uninit(void)
{
	//オブジェクトの破棄
	SetDeathFlag();
}

//=============================================================================
// 更新処理
//=============================================================================
void CPlayer::Update(void)
{
	// キーボード更新
	CInputKeyboard *pKeyboard = CManager::GetKeyboard();

	// 古い座標取得
	m_OldPos = m_pos;

	// 重力をかける
	m_move.y -= GRAVITY_POWAR;
	m_pos.y += m_move.y;		// 落下

	// 地面の制限
	GroundLimit();

	//位置へ移動量を加算
	m_pos += m_move;

	while (m_rotDest.y - m_rot.y > D3DXToRadian(180))
	{
		m_rotDest.y -= D3DXToRadian(360);
	}

	while (m_rotDest.y - m_rot.y < D3DXToRadian(-180))
	{
		m_rotDest.y += D3DXToRadian(360);
	}

	m_rot += (m_rotDest - m_rot)*0.1f;

	// Wキーを押したとき
	if (pKeyboard->GetPress(DIK_W))
	{
		m_rotDest.y = D3DXToDegree(m_fAngle);
		m_pos.z += cosf(0)*PLAYER_SPEED;
	}

	// Sキーを押したとき
	if (pKeyboard->GetPress(DIK_S))
	{

		m_rotDest.y = sin(m_fAngle);
		m_pos.z += cosf(D3DX_PI)*PLAYER_SPEED;
	}
	// Aキーを押したとき
	if (pKeyboard->GetPress(DIK_A))
	{
		m_pos.x += sinf(-D3DX_PI / 2)*PLAYER_SPEED;
	}
	// Dキーを押したとき
	if (pKeyboard->GetPress(DIK_D))
	{
		m_pos.x += sinf(D3DX_PI / 2)*PLAYER_SPEED;
	}

	//範囲外に行かないようにする処理
	MapLimit();

}

//=============================================================================
// 描画処理
//=============================================================================
void CPlayer::Draw(void)
{
	// 描画処理
	//デバイス情報の取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans, mtxScale;

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);					// 裏面を（左回り）をカリング

	for (int nCntModelNum = 0; nCntModelNum < MAX_MODEL_PARTS; nCntModelNum++)
	{
		//if (m_apModelAnime[nCntModelNum] != NULL)
		//{

		//	//階層モデルクラスの描画処理
		//	m_apModelAnime[nCntModelNum]->Draw();
		//}
	}

	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);					// 裏面を（左回り）をカリング
}

//=============================================================================
// 地面の制限
//=============================================================================
void CPlayer::GroundLimit(void)
{
	if (m_pos.y <= GROUND_RIMIT)
	{
		m_move.y = 0.0f;
		m_pos.y = GROUND_RIMIT;
	}
}

//=============================================================================
// 範囲外に行かないようにする処理
//=============================================================================

void CPlayer::MapLimit(void)
{
	//右
	if (m_pos.x > MAP_LIMIT)
	{
		m_pos.x = MAP_LIMIT;
	}

	//左
	if (m_pos.x <-MAP_LIMIT)
	{
		m_pos.x = -MAP_LIMIT;
	}

	//奥
	if (m_pos.z > MAP_LIMIT)
	{
		m_pos.z = MAP_LIMIT;
	}

	//手前
	if (m_pos.z <-MAP_LIMIT)
	{
		m_pos.z = -MAP_LIMIT;
	}
}

//=============================================================================
// ファイル読み込み処理
//=============================================================================
HRESULT CPlayer::ReadFile(void)
{
	
	//D3DXLoadMeshHierarchyFromX(TEST_XFAILE_NAME,);

	//FILE *pFile = NULL;	//FILEポインタ
	//char aHeadData[1024];
	//char aModeName[1024];
	//int nModelIndex = 0;	//モデルのインデックス
	//int nMotionType = 0;	//モーションのタイプ
	//int nKeyNum = 0;		//キー番号
	//int nMotionNum = 0;		//モーション番号
	//if (m_nPlayerNum == 0)
	//{
	//	//ファイルオープン
	//	pFile = fopen(LBX_XFAILE_NAME, "r");
	//}
	//else
	//{
	//	//ファイルオープン
	//	pFile = fopen(GANDAMU_XFAILE_NAME, "r");
	//}

	//if (pFile != NULL)
	//{
	//	do
	//	{
	//		//一列読み込んでモード情報を抽出
	//		fgets(aHeadData, sizeof(aHeadData), pFile);
	//		sscanf(aHeadData, "%s", aModeName);

	//		if (strcmp(aModeName, "MODEL_FILENAME") == 0)
	//		{
	//			//Xファイルの名前
	//			sscanf(aHeadData, "%*s %*s %s %*s %*s", g_modelfile[nModelIndex].xFileName);

	//			//インデックスを１つ進める
	//			nModelIndex++;
	//		}

	//		if (strcmp(aModeName, "CHARACTERSET") == 0)
	//		{
	//			//インデックスを最初に戻す
	//			nModelIndex = 0;

	//			//END_MOTIONSETを読み込むまで繰り返す
	//			while (strcmp(aModeName, "END_CHARACTERSET") != 0)
	//			{
	//				//一列読み込んでモード情報を抽出
	//				fgets(aHeadData, sizeof(aHeadData), pFile);
	//				sscanf(aHeadData, "%s", aModeName);

	//				if (strcmp(aModeName, "PARTSSET") == 0)
	//				{
	//					//END_PARTSSETを読み込むまで繰り返す
	//					while (strcmp(aModeName, "END_PARTSSET") != 0)
	//					{
	//						//一列読み込んでモード情報を抽出
	//						fgets(aHeadData, sizeof(aHeadData), pFile);
	//						sscanf(aHeadData, "%s", aModeName);

	//						if (strcmp(aModeName, "PARENT") == 0)
	//						{
	//							//親子情報の設定
	//							sscanf(aHeadData, "%*s %*s %d", &g_modelfile[nModelIndex].nParent);
	//						}
	//						if (strcmp(aModeName, "POS") == 0)
	//						{
	//							//位置の設定
	//							sscanf(aHeadData, "%*s %*s %f %f %f", &g_modelfile[nModelIndex].offsetPos.x,
	//								&g_modelfile[nModelIndex].offsetPos.y, &g_modelfile[nModelIndex].offsetPos.z);
	//						}
	//						if (strcmp(aModeName, "ROT") == 0)
	//						{
	//							//向きの設定
	//							sscanf(aHeadData, "%*s %*s %f %f %f", &g_modelfile[nModelIndex].offsetRot.x,
	//								&g_modelfile[nModelIndex].offsetRot.y, &g_modelfile[nModelIndex].offsetRot.z);
	//						}
	//					}
	//					//インデックスを１つ進める
	//					nModelIndex++;
	//				}
	//			}
	//		}

	//		//モーションセット
	//		if (strcmp(aModeName, "MOTIONSET") == 0)
	//		{
	//			//END_MOTIONSETを読み込むまで繰り返す
	//			while (strcmp(aModeName, "END_MOTIONSET") != 0)
	//			{
	//				//一列読み込んでモード情報を抽出
	//				fgets(aHeadData, sizeof(aHeadData), pFile);
	//				sscanf(aHeadData, "%s", aModeName);

	//				//ループ情報の取得
	//				if (strcmp(aModeName, "LOOP") == 0)
	//				{
	//					sscanf(aHeadData, "%*s %*s %d", (int*)&m_Motion[nMotionType].bLoop);
	//				}

	//				//キー情報の取得
	//				if (strcmp(aModeName, "NUM_KEY") == 0)
	//				{
	//					sscanf(aHeadData, "%*s %*s %d", (int*)&m_Motion[nMotionType].nNumKey);
	//				}

	//				if (strcmp(aModeName, "KEYSET") == 0)
	//				{
	//					//END_KEYSETになるまで繰り返す
	//					while (strcmp(aModeName, "END_KEYSET") != 0)
	//					{
	//						//一列読み込んでモード情報を抽出
	//						fgets(aHeadData, sizeof(aHeadData), pFile);
	//						sscanf(aHeadData, "%s", aModeName);

	//						//フレーム数の取得
	//						if (strcmp(aModeName, "FRAME") == 0)
	//						{
	//							sscanf(aHeadData, "%*s %*s %d", &m_Motion[nMotionType].aKeyInfo[nMotionNum].nFrame);
	//						}

	//						//各キーのオフセット情報の取得
	//						if (strcmp(aModeName, "KEY") == 0)
	//						{
	//							//END_KEYになるまで繰り返す
	//							while (strcmp(aModeName, "END_KEY") != 0)
	//							{
	//								//一列読み込んでモード情報を抽出
	//								fgets(aHeadData, sizeof(aHeadData), pFile);
	//								sscanf(aHeadData, "%s", aModeName);

	//								//位置の取得
	//								if (strcmp(aModeName, "POS") == 0)
	//								{
	//									sscanf(aHeadData, "%*s %*s %f %f %f",
	//										&m_Motion[nMotionType].aKeyInfo[nMotionNum].aKey[nKeyNum].fPosX,
	//										&m_Motion[nMotionType].aKeyInfo[nMotionNum].aKey[nKeyNum].fPosY,
	//										&m_Motion[nMotionType].aKeyInfo[nMotionNum].aKey[nKeyNum].fPosZ);
	//								}

	//								//向きの取得
	//								if (strcmp(aModeName, "ROT") == 0)
	//								{
	//									sscanf(aHeadData, "%*s %*s %f %f %f",
	//										&m_Motion[nMotionType].aKeyInfo[nMotionNum].aKey[nKeyNum].fRotX,
	//										&m_Motion[nMotionType].aKeyInfo[nMotionNum].aKey[nKeyNum].fRotY,
	//										&m_Motion[nMotionType].aKeyInfo[nMotionNum].aKey[nKeyNum].fRotZ);
	//								}
	//							}
	//							//読み終わったらカウントを1つ進める
	//							nKeyNum++;
	//						}
	//					}
	//					nKeyNum = 0;
	//					nMotionNum++;
	//				}
	//			}
	//			nMotionNum = 0;
	//			nMotionType++;
	//		}
	//	} while (strcmp(aModeName, "END_SCRIPT") != 0);

	//	//ファイルクローズ
	//	fclose(pFile);

	//	return S_OK;
	//}
	//else
	//{
		//失敗した場合メッセージボックスを表示
		MessageBox(NULL, "モーションファイルを開くのに失敗しました", "警告", MB_OK | MB_ICONEXCLAMATION);

		return	E_FAIL;
	//}
}

void CPlayer::SetPos(D3DXVECTOR3 pos)
{
	m_pos = pos;
}

//=============================================================================
// 座標の情報
//=============================================================================
D3DXVECTOR3 CPlayer::GetPos(void)
{
	return m_pos;
}

//=============================================================================
// 古い座標情報
//=============================================================================
D3DXVECTOR3 CPlayer::GetOldPos(void)
{
	return m_OldPos;
}

//=============================================================================
// 移動量の設定
//=============================================================================
void CPlayer::SetMove(D3DXVECTOR3 move)
{
	m_move = move;
}

D3DXVECTOR3 CPlayer::GetMove(void)
{
	return m_move;
}

D3DXVECTOR3 CPlayer::GetRot(void)
{
	return m_rot;
}