#ifndef _PLAYER_H_
#define _PLAYER_H_
//=============================================================================
//
// �v���C���[�N���X�w�b�_�[ [player.h]
// Author : Konishi Yuuto
//
//=============================================================================

//=============================================================================
// �C���N���[�h
//=============================================================================
#include "scene.h"
//#include "modelanime.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define LIFE_NUM			(2)			// �\�����郉�C�t�̐�
#define WIN_LOSE_NUM		(2)
#define MAX_PARTS			(10)		// �p�[�c�̐�
#define MOTION_KEYSET_MAX	(32)		// �L�[�Z�b�g�ő吔

#define PLAYER2_POS_X		(0.0f)		// ���W
#define PLAYER2_POS_Y		(171.0f)	// ���W
#define PLAYER2_POS_Z		(500)		// ���W

#define PLAYER_SIZE_X		(1)			// �T�C�Y
#define PLAYER_SIZE_Y		(1)			// �T�C�Y
#define PLAYER_SIZE_Z		(1)			// �T�C�Y

#define MAX_MODEL_PARTS			(21)		//���f���̃p�[�c�̍ő吔

#define PLAYER_COLLISION_X	(200)		// �����蔻��
#define PLAYER_COLLISION_Y	(175)		// �����蔻��
#define PLAYER_COLLISION_Z	(200)		// �����蔻��

#define WEAPON_COLLISION_X	(100)		// ����̓����蔻��
#define WEAPON_COLLISION_Y	(50)		// ����̓����蔻��
#define WEAPON_COLLISION_Z	(100)		// ����̓����蔻��

#define PLAYER_RADIUS		(150)		// ���a
#define PLAYER_BOMB			(80)		// �{�����̂ɕK�v�ȃQ�[�W��
#define PLAYER_LASER		(35)		// ���[�U�[���̂ɕK�v�ȃQ�[�W��

#define MAX_BOOST			(2)

#define MAP_LIMIT			(6750)		// �s��������
//=============================================================================
// �O���錾
//=============================================================================

//=============================================================================
//�@���f���t�@�C�����̍\����
//=============================================================================
typedef struct
{
	char xFileName[1024];	//�t�@�C���l�[��
	D3DXVECTOR3 offsetPos;	//�ʒu�̃I�t�Z�b�g���
	D3DXVECTOR3 offsetRot;	//�����̃I�t�Z�b�g���
	int nParent;	//�e���
}MODELFILLE;

//=============================================================================
//�@�e�v�f�̃L�[���
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
//	�L�[���̍\����
//=============================================================================
typedef struct
{
	int nFrame;	//�t���[����
	KEY aKey[MAX_MODEL_PARTS];	//�e�p�[�c�̃L�[���
}KEY_INFO;

//=============================================================================
//�@���[�V�������̍\����
//=============================================================================
typedef struct
{
	bool bLoop;	//���[�v���邩�ǂ���
	int nNumKey;	//�L�[��
	KEY_INFO aKeyInfo[20];	//�L�[���
}Motion_Info;

//=============================================================================
// �v���C���[�N���X
//=============================================================================
class CPlayer : public CScene
{
public:

	typedef enum
	{
		PLAYER_STATE_NONE = 0,		// �����u
		PLAYER_STATE_NORMAL,		// �ʏ���
		PLAYER_STATE_DAMAGE,		// �_���[�W
		PLAYER_STATE_EXPLOSION,		// ����
		PLAYER_STATE_DRAW,			// ��������
		PLAYER_STATE_MAX
	}PLAYER_STATE;

	CPlayer(int nPriority = PRIORITY_PLAYER);						// �R���X�g���N�^
	~CPlayer();						// �f�X�g���N�^

	static CPlayer*Create(D3DXVECTOR3 pos, D3DXVECTOR3 size);		// �N���G�C�g

	HRESULT Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 size);	// ����������
	void Uninit(void);												// �I������
	void Update(void);												// �X�V����
	void Draw(void);												// �`�揈��

	void SetPos(D3DXVECTOR3 pos);
	D3DXVECTOR3 GetPos(void);										// ���݂̍��W���
	D3DXVECTOR3 GetOldPos(void);									// �Â����W���
	D3DXVECTOR3 GetRot(void);										// �p�x���
	void SetMove(D3DXVECTOR3 move);										// �ړ��ʂ̐ݒ�
	D3DXVECTOR3 GetMove(void);
	HRESULT ReadFile(void);
	void GroundLimit(void);
	void MapLimit(void);
private:
	D3DXVECTOR3 m_pos;						// ���W
	D3DXVECTOR3 m_OldPos;					// 1�t���[���O�̍��W
	D3DXVECTOR3 m_rot;						// ��](���ݒn)
	D3DXVECTOR3 m_rotDest;					// ��](�ڕW�l)
	D3DXVECTOR3 m_move;						// �ړ�
	float m_fAngle;							// �p�x
	D3DXMATRIX m_mtxWorld;						// ���[���h�}�g���b�N�X
	//CModelAnime *m_apModelAnime[MAX_MODEL_PARTS];	//���f���p�[�c�p�̃|�C���^
	static int m_nPlayerAll;					// �v���C���[�̐�
	int m_nPlayerNum;							// �v���C���[�̔ԍ�
};
#endif