#include "Animation.h"
#include <vector>

/*class CHighLevelAnimController
{
public:
	// アニメーションコントローラを設定
	bool SetAnimationController(ID3DXAnimationController *pAnimCont);
	// アニメーションコントローラを取得
	bool GetAnimationController(ID3DXAnimationController **ppAnimCont);
	// ループ時間を設定
	bool SetLoopTime(UINT animID, FLOAT time);
	// 動作開始にかかる時間を設定
	bool SetShiftTime(UINT animID, FLOAT interval);
	// アニメーションを切り替え
	bool ChangeAnimation(UINT animID);
	// アニメーションを更新
	bool AdvanceTime(FLOAT time);

	HLANIMATION_DESC m_Anim;
	ID3DXAnimationController *m_AnimCont;
	DWORD m_CurAnimID;
	DWORD m_PreAnimID;
};

struct HLANIMATION_DESC
{
	UINT uiAnimID;                 // アニメーションID
	ID3DXAnimationSet *pAnimSet;   // アニメーションセット
	FLOAT fLoopTime;               // 1ループの時間
	FLOAT fTrackSpeed;             // トラックスピード調節値
	FLOAT fShiftTime;              // シフトするのにかかる時間
	FLOAT fCurWeightTime;          // 現在のウェイト時間
};

bool CHighLevelAnimController::SetAnimationController(ID3DXAnimationController * pAnimCont)
{


	return false;
}

bool CHighLevelAnimController::GetAnimationController(ID3DXAnimationController ** ppAnimCont)
{
	return false;
}

bool CHighLevelAnimController::SetLoopTime(UINT animID, FLOAT time)
{
	// 指定のアニメーションIDの存在をチェック
	if (m_Anim.size() <= animID)
		return false;

	// トラックスピード調節値を算出
	FLOAT DefTime = m_Anim[animID].pAnimSet->GetPeriod();
	m_Anim[animID].fLoopTime = time;
	m_Anim[animID].fTrackSpeed = DefTime / time;

	return true;
}

bool CHighLevelAnimController::SetShiftTime(UINT animID, FLOAT interval)
{
	// 指定のアニメーションIDの存在をチェック
	if (m_Anim.size() <= animID)
		return false;

	// シフト時間を登録
	m_Anim[animID].fShiftTime = interval;

	return true;
}

bool CHighLevelAnimController::ChangeAnimation(UINT animID)
{
	// 指定のアニメーションIDの存在をチェック
	if (m_Anim.size() <= animID)
		return false;

	// 異なるアニメーションであるかをチェック
	if (m_CurAnimID == animID)
		return false;

	// 現在のアニメーションセットの設定値を取得
	D3DXTRACK_DESC TD;   // トラックの能力
	m_AnimCont->GetTrackDesc(0, &TD);

	// 今のアニメーションをトラック1に移行し
	// トラックの設定値も移行
	m_AnimCont->SetTrackAnimationSet(1, m_Anim[m_CurAnimID].pAnimSet);
	m_AnimCont->SetTrackDesc(1, &TD);

	// 新しいアニメーションセットをトラック0に設定
	m_AnimCont->SetTrackAnimationSet(0, m_Anim[animID].pAnimSet);

	// トラック0のスピードの設定
	m_AnimCont->SetTrackSpeed(0, m_Anim[animID].fTrackSpeed);

	// トラックの合成を許可
	m_AnimCont->SetTrackEnable(0, true);
	m_AnimCont->SetTrackEnable(1, true);

	// ウェイト時間を初期化
	m_Anim[animID].fCurWeightTime = 0.0f;

	// 現在のアニメーション番号を切り替え
	m_PreAnimID = m_CurAnimID;
	m_CurAnimID = animID;

	return true;
}

bool CHighLevelAnimController::AdvanceTime(FLOAT time)
{
	// 合成中か否かを判定
	m_Anim[m_CurAnimID].fCurWeightTime += time;
	if (m_Anim[m_CurAnimID].fCurWeightTime <= m_Anim[m_CurAnimID].fShiftTime)
	{
		// 合成中。ウェイトを算出
		FLOAT Weight = m_Anim[m_CurAnimID].fCurWeightTime / m_Anim[m_CurAnimID].fShiftTime;
		// ウェイトを登録
		m_AnimCont->SetTrackWeight(0, Weight);       // 現在のアニメーション
		m_AnimCont->SetTrackWeight(1, 1 - Weight);   // 前のアニメーション
	}
	else
	{
		// 合成終了中。通常アニメーションをするTrack0のウェイトを最大値に
		m_AnimCont->SetTrackWeight(0, 1);       // 現在のアニメーション
		m_AnimCont->SetTrackEnable(1, false);      // 前のアニメーションを無効にする
	}

	// 時間を更新
	m_AnimCont->AdvanceTime(time, NULL);

	return true;
}*/