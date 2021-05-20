#include "Animation.h"
#include <vector>

/*class CHighLevelAnimController
{
public:
	// �A�j���[�V�����R���g���[����ݒ�
	bool SetAnimationController(ID3DXAnimationController *pAnimCont);
	// �A�j���[�V�����R���g���[�����擾
	bool GetAnimationController(ID3DXAnimationController **ppAnimCont);
	// ���[�v���Ԃ�ݒ�
	bool SetLoopTime(UINT animID, FLOAT time);
	// ����J�n�ɂ����鎞�Ԃ�ݒ�
	bool SetShiftTime(UINT animID, FLOAT interval);
	// �A�j���[�V������؂�ւ�
	bool ChangeAnimation(UINT animID);
	// �A�j���[�V�������X�V
	bool AdvanceTime(FLOAT time);

	HLANIMATION_DESC m_Anim;
	ID3DXAnimationController *m_AnimCont;
	DWORD m_CurAnimID;
	DWORD m_PreAnimID;
};

struct HLANIMATION_DESC
{
	UINT uiAnimID;                 // �A�j���[�V����ID
	ID3DXAnimationSet *pAnimSet;   // �A�j���[�V�����Z�b�g
	FLOAT fLoopTime;               // 1���[�v�̎���
	FLOAT fTrackSpeed;             // �g���b�N�X�s�[�h���ߒl
	FLOAT fShiftTime;              // �V�t�g����̂ɂ����鎞��
	FLOAT fCurWeightTime;          // ���݂̃E�F�C�g����
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
	// �w��̃A�j���[�V����ID�̑��݂��`�F�b�N
	if (m_Anim.size() <= animID)
		return false;

	// �g���b�N�X�s�[�h���ߒl���Z�o
	FLOAT DefTime = m_Anim[animID].pAnimSet->GetPeriod();
	m_Anim[animID].fLoopTime = time;
	m_Anim[animID].fTrackSpeed = DefTime / time;

	return true;
}

bool CHighLevelAnimController::SetShiftTime(UINT animID, FLOAT interval)
{
	// �w��̃A�j���[�V����ID�̑��݂��`�F�b�N
	if (m_Anim.size() <= animID)
		return false;

	// �V�t�g���Ԃ�o�^
	m_Anim[animID].fShiftTime = interval;

	return true;
}

bool CHighLevelAnimController::ChangeAnimation(UINT animID)
{
	// �w��̃A�j���[�V����ID�̑��݂��`�F�b�N
	if (m_Anim.size() <= animID)
		return false;

	// �قȂ�A�j���[�V�����ł��邩���`�F�b�N
	if (m_CurAnimID == animID)
		return false;

	// ���݂̃A�j���[�V�����Z�b�g�̐ݒ�l���擾
	D3DXTRACK_DESC TD;   // �g���b�N�̔\��
	m_AnimCont->GetTrackDesc(0, &TD);

	// ���̃A�j���[�V�������g���b�N1�Ɉڍs��
	// �g���b�N�̐ݒ�l���ڍs
	m_AnimCont->SetTrackAnimationSet(1, m_Anim[m_CurAnimID].pAnimSet);
	m_AnimCont->SetTrackDesc(1, &TD);

	// �V�����A�j���[�V�����Z�b�g���g���b�N0�ɐݒ�
	m_AnimCont->SetTrackAnimationSet(0, m_Anim[animID].pAnimSet);

	// �g���b�N0�̃X�s�[�h�̐ݒ�
	m_AnimCont->SetTrackSpeed(0, m_Anim[animID].fTrackSpeed);

	// �g���b�N�̍���������
	m_AnimCont->SetTrackEnable(0, true);
	m_AnimCont->SetTrackEnable(1, true);

	// �E�F�C�g���Ԃ�������
	m_Anim[animID].fCurWeightTime = 0.0f;

	// ���݂̃A�j���[�V�����ԍ���؂�ւ�
	m_PreAnimID = m_CurAnimID;
	m_CurAnimID = animID;

	return true;
}

bool CHighLevelAnimController::AdvanceTime(FLOAT time)
{
	// ���������ۂ��𔻒�
	m_Anim[m_CurAnimID].fCurWeightTime += time;
	if (m_Anim[m_CurAnimID].fCurWeightTime <= m_Anim[m_CurAnimID].fShiftTime)
	{
		// �������B�E�F�C�g���Z�o
		FLOAT Weight = m_Anim[m_CurAnimID].fCurWeightTime / m_Anim[m_CurAnimID].fShiftTime;
		// �E�F�C�g��o�^
		m_AnimCont->SetTrackWeight(0, Weight);       // ���݂̃A�j���[�V����
		m_AnimCont->SetTrackWeight(1, 1 - Weight);   // �O�̃A�j���[�V����
	}
	else
	{
		// �����I�����B�ʏ�A�j���[�V����������Track0�̃E�F�C�g���ő�l��
		m_AnimCont->SetTrackWeight(0, 1);       // ���݂̃A�j���[�V����
		m_AnimCont->SetTrackEnable(1, false);      // �O�̃A�j���[�V�����𖳌��ɂ���
	}

	// ���Ԃ��X�V
	m_AnimCont->AdvanceTime(time, NULL);

	return true;
}*/