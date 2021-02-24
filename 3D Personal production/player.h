#ifndef _PLAYER_H_
#define _PLAYER_H_
//=============================================================================
//
// プレイヤークラスヘッダー [player.h]
// Author : Konishi Yuuto
//
//=============================================================================

//=============================================================================
// インクルード
//=============================================================================
#include "scene.h"
//#include "modelanime.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define LIFE_NUM			(2)			// 表示するライフの数
#define WIN_LOSE_NUM		(2)
#define MAX_PARTS			(10)		// パーツの数
#define MOTION_KEYSET_MAX	(32)		// キーセット最大数

#define PLAYER2_POS_X		(0.0f)		// 座標
#define PLAYER2_POS_Y		(171.0f)	// 座標
#define PLAYER2_POS_Z		(500)		// 座標

#define PLAYER_SIZE_X		(1)			// サイズ
#define PLAYER_SIZE_Y		(1)			// サイズ
#define PLAYER_SIZE_Z		(1)			// サイズ

#define MAX_MODEL_PARTS			(21)		//モデルのパーツの最大数

#define PLAYER_COLLISION_X	(200)		// 当たり判定
#define PLAYER_COLLISION_Y	(175)		// 当たり判定
#define PLAYER_COLLISION_Z	(200)		// 当たり判定

#define WEAPON_COLLISION_X	(100)		// 武器の当たり判定
#define WEAPON_COLLISION_Y	(50)		// 武器の当たり判定
#define WEAPON_COLLISION_Z	(100)		// 武器の当たり判定

#define PLAYER_RADIUS		(150)		// 半径
#define PLAYER_BOMB			(80)		// ボム撃つのに必要なゲージ数
#define PLAYER_LASER		(35)		// レーザー撃つのに必要なゲージ数

#define MAX_BOOST			(2)

#define MAP_LIMIT			(6750)		// 行ける上限数
//=============================================================================
// 前方宣言
//=============================================================================

//=============================================================================
//　モデルファイル情報の構造体
//=============================================================================
typedef struct
{
	char xFileName[1024];	//ファイルネーム
	D3DXVECTOR3 offsetPos;	//位置のオフセット情報
	D3DXVECTOR3 offsetRot;	//向きのオフセット情報
	int nParent;	//親情報
}MODELFILLE;

//=============================================================================
//　各要素のキー情報
//=============================================================================
typedef struct
{
	float fPosX;
	float fPosY;
	float fPosZ;
	float fRotX;
	float fRotY;
	float fRotZ;
}KEY;

//=============================================================================
//	キー情報の構造体
//=============================================================================
typedef struct
{
	int nFrame;	//フレーム数
	KEY aKey[MAX_MODEL_PARTS];	//各パーツのキー情報
}KEY_INFO;

//=============================================================================
//　モーション情報の構造体
//=============================================================================
typedef struct
{
	bool bLoop;	//ループするかどうか
	int nNumKey;	//キー数
	KEY_INFO aKeyInfo[20];	//キー情報
}Motion_Info;

//=============================================================================
// プレイヤークラス
//=============================================================================
class CPlayer : public CScene
{
public:

	typedef enum
	{
		PLAYER_STATE_NONE = 0,		// 初期置
		PLAYER_STATE_NORMAL,		// 通常状態
		PLAYER_STATE_DAMAGE,		// ダメージ
		PLAYER_STATE_EXPLOSION,		// 爆発
		PLAYER_STATE_DRAW,			// 引き分け
		PLAYER_STATE_MAX
	}PLAYER_STATE;

	CPlayer(int nPriority = PRIORITY_PLAYER);						// コンストラクタ
	~CPlayer();						// デストラクタ

	static CPlayer*Create(D3DXVECTOR3 pos, D3DXVECTOR3 size);		// クリエイト

	HRESULT Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 size);	// 初期化処理
	void Uninit(void);												// 終了処理
	void Update(void);												// 更新処理
	void Draw(void);												// 描画処理

	void SetPos(D3DXVECTOR3 pos);
	D3DXVECTOR3 GetPos(void);										// 現在の座標情報
	D3DXVECTOR3 GetOldPos(void);									// 古い座標情報
	D3DXVECTOR3 GetRot(void);										// 角度情報
	void SetMove(D3DXVECTOR3 move);										// 移動量の設定
	D3DXVECTOR3 GetMove(void);
	HRESULT ReadFile(void);
	void GroundLimit(void);
	void MapLimit(void);
private:
	D3DXVECTOR3 m_pos;						// 座標
	D3DXVECTOR3 m_OldPos;					// 1フレーム前の座標
	D3DXVECTOR3 m_rot;						// 回転(現在地)
	D3DXVECTOR3 m_rotDest;					// 回転(目標値)
	D3DXVECTOR3 m_move;						// 移動
	float m_fAngle;							// 角度
	D3DXMATRIX m_mtxWorld;						// ワールドマトリックス
	//CModelAnime *m_apModelAnime[MAX_MODEL_PARTS];	//モデルパーツ用のポインタ
	static int m_nPlayerAll;					// プレイヤーの数
	int m_nPlayerNum;							// プレイヤーの番号
};
#endif