//=============================================================================
//
// ���C������ [camera.cpp]
// Author : �����ߔ��u
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
//�}�N����`
//=============================================================================
#define CAMERA_DEFAULT_F��			(D3DXToRadian(75.0f))	//�J�����̃�Default�l
#define DISTANCE					(1300.0f)				//���_�`�����_�̋���
#define PLAYER_HEIGHT				(200.0f)				//�����_�̍���
#define PLAYE_ROT_VERTICAL_FRONT	(D3DXToRadian(0.0f))	//�v���C���[�̏c���O
#define WIN_START_ROT_VERTICAL		(D3DXToRadian(180.0f))	//�������c����]�̊J�n�p
#define WIN_START_ROT_HORIZONTAL	(D3DXToRadian(40.0f))	//������������]�̊J�n�p
#define WIN_START_DISTANCE			(1500.0f)				//�������̊J�n����

#define WIN_END_ROT_VERTICAL		(D3DXToRadian(360.0f+180.0f))	//�������c����]�̏I���p
#define WIN_END_ROT_HORIZONTAL		(D3DXToRadian(100.0f))	//������������]�̏I���p
#define WIN_END_DISTANCE			(500.0f)				//�������̏I������
#define WIN_ROT_MOVE_SUBTRACTION	(0.2f)					//�������̉�����]�p�x�ω���

#define LOSE_START_ROT_VERTICAL		(D3DXToRadian(180.0f))	//�s�k���c����]�̊J�n�p
#define LOSE_START_ROT_HORIZONTAL	(D3DXToRadian(60.0f))	//�s�k��������]�̊J�n�p
#define LOSE_START_DISTANCE			(500.0f)				//�s�k���̊J�n����
#define LOSE_END_DISTANCE			(1000.0f)				//�s�k���̏I������
#define LOSE_MOVE_SUBTRACTION		(0.2f)					//�s�k���̉�����]�p�x�ω���

#define CAMERA_MIN_F��				(D3DXToRadian(10.0f))	//�J�����̍ŏ��p
#define CAMERA_MAX_F��				(D3DXToRadian(170.0f))	//�J�����̍ő�p
#define CAMERA_MIN_HIGHT			(2.0f)					//�J�����̍Œፂ�x
#define STICK_SENSITIVITY			(50.0f)					//�X�e�B�b�N���x
#define STICK_INPUT_CONVERSION		(D3DXToRadian(1.0f))	//�X�e�B�b�N���͕ω���
#define JUDGMENT_RANGE				(50.0f)					//���苗��

//=============================================================================
// static������
//=============================================================================
int CCamera::m_nCameraAll = 0;

//=============================================================================
// �N���G�C�g
//=============================================================================
CCamera * CCamera::Create(void)
{
	// ����������
	CCamera *pCamera = new CCamera;

	pCamera->Init();

	return pCamera;
}

//=============================================================================
//�J�����N���X�̃R���X�g���N�^
//=============================================================================
CCamera::CCamera()
{
	//�e�����o�ϐ��̃N���A
	m_posV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �J�����̍��W
	m_posVDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �J�����̍��W�i�ړI�n�j
	m_posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �����_
	m_posRDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �����_�i�ړI�n�j
	m_posU = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// ������x�N�g��
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// ����
	m_fDistance = 0.0f;							// ���_�`�����_�̋���
	m_fMove = 0.0f;								// �ړ���
	m_nCameraNum = m_nCameraAll++;				// �J�����̔ԍ�
	m_state = CAMERASTATE_NONE;					//�X�e�[�^�X������
}

//=============================================================================
//�J�����N���X�̃f�X�g���N�^
//=============================================================================
CCamera::~CCamera()
{
}

//=============================================================================
//�J�����N���X�̏���������
//=============================================================================
HRESULT CCamera::Init(void)
{
	// �����_���[�̏����󂯎��
	CRenderer *pRenderer = NULL;
	pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	switch (m_nCameraNum)
	{
	case 0:
		m_fMove = 5.0f;
		m_fDistance = DISTANCE;
		m_rot.y = 0.0f;
		m_f�� = CAMERA_DEFAULT_F��;
		m_f�� = D3DXToRadian(180.0f);						// �����l�G�̂ق�������
		m_posR = D3DXVECTOR3(0.0f, PLAYER_HEIGHT, 0.0f);	// �����_�ݒ�
		m_posU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);				// ������x�N�g��
		m_posV.x = m_posR.x + m_fDistance* sinf(m_f��) * sinf(m_f��);	//�J�����ʒuX
		m_posV.y = m_posR.z + m_fDistance* cosf(m_f��);					//�J�����ʒuY
		m_posV.z = m_posR.y + m_fDistance* sinf(m_f��) * cosf(m_f��);	//�J�����ʒuZ
		break;

	case 1:
		m_fMove = 5.0f;
		m_fDistance = DISTANCE;
		m_f�� = CAMERA_DEFAULT_F��;
		m_f�� = D3DXToRadian(0.0f);									// �����l�G�̂ق�������
		m_posR = D3DXVECTOR3(0.0f, PLAYER_HEIGHT, 0.0f);			// �����_�ݒ�
		m_posU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);						// ������x�N�g��
		m_posV.x = m_posR.x + m_fDistance* sinf(m_f��) * sinf(m_f��);	//�J�����ʒuX
		m_posV.y = m_posR.z + m_fDistance* cosf(m_f��);					//�J�����ʒuY
		m_posV.z = m_posR.y + m_fDistance* sinf(m_f��) * cosf(m_f��);	//�J�����ʒuZ
		break;

	default:
		break;
	}

	//��Ԃ̏����ݒ�
	m_state = CAMERASTATE_NORMAL;

	return S_OK;
}

//=============================================================================
//�J�����N���X�̏I������
//=============================================================================
void CCamera::Uninit(void)
{
	m_nCameraAll = 0;
}

//=============================================================================
//�J�����N���X�̍X�V����
//=============================================================================
void CCamera::Update(void)
{
	static bool bPlayerWin = false;	//�v���C���[�����t���O
	static int nWinPlayer = 0;		//Win�v���C���[�ԍ�

									//�L�[�{�[�h�N���X���̎擾
	CInputKeyboard *pKeyInput = CManager::GetKeyboard();

	D3DXVECTOR3 PlayerPos[MAX_PLAYER];	//�v���C���[�ʒu
	D3DXVECTOR3 PlayerRot[MAX_PLAYER];	//�v���C���[�p�x
	CPlayer::PLAYER_STATE PlayerState[MAX_PLAYER];
	bool PlayerWin[MAX_PLAYER];

	if (CGame::GetPlayer(m_nCameraNum) != NULL)
	{
		//�v���C���[1�ʒu�擾
		PlayerPos[m_nCameraNum] = CGame::GetPlayer(m_nCameraNum)->GetPos();
		//�v���C���[1�p�x�擾
		PlayerRot[m_nCameraNum] = CGame::GetPlayer(m_nCameraNum)->GetRot();

		switch (m_state)
		{
		case CAMERASTATE_NONE:
			break;

		case CAMERASTATE_NORMAL://�ʏ�
			NomalUpdate(PlayerPos, PlayerRot);
			break;

		case CAMERASTATE_MAX://STATE�̍ő吔
			break;

		default:
			break;
		}
	}
}

//=============================================================================
//�J�����N���X�̎������X�V����
//=============================================================================
void CCamera::NomalUpdate(D3DXVECTOR3 PlayerPos[], D3DXVECTOR3 PlayerRot[])
{

	//�L�[�{�[�h�N���X���̎擾
	CInputKeyboard *pKeyInput = CManager::GetKeyboard();

		//���_�i�J�������W�j�̍�����
		if (pKeyInput->GetPress(DIK_LEFT))
		{
			m_f�� += STICK_INPUT_CONVERSION;
		}
		//���_�i�J�������W�j�̉E����
		if (pKeyInput->GetPress(DIK_RIGHT))
		{
			m_f�� -= STICK_INPUT_CONVERSION;
		}
		//���_�i�J�������W�j�̏����
		if (pKeyInput->GetPress(DIK_UP) && m_f�� >= CAMERA_MIN_F��)
		{
			m_f�� -= STICK_INPUT_CONVERSION;
		}
		//���_�i�J�������W�j�̉�����
		if (pKeyInput->GetPress(DIK_DOWN) && m_f�� <= CAMERA_MAX_F��)
		{
			m_f�� += STICK_INPUT_CONVERSION;
		}

		m_posVDest.x = PlayerPos[m_nCameraNum].x + m_fDistance * sinf(m_f��) * sinf(m_f��);	//�J�����ʒuX�ݒ�
		m_posVDest.y = PlayerPos[m_nCameraNum].y + PLAYER_HEIGHT + m_fDistance * cosf(m_f��);				//�J�����ʒuY�ݒ�
		m_posVDest.z = PlayerPos[m_nCameraNum].z + m_fDistance * sinf(m_f��) * cosf(m_f��);	//�J�����ʒuZ�ݒ�

		m_posRDest = D3DXVECTOR3(PlayerPos[m_nCameraNum].x, PlayerPos[m_nCameraNum].y + PLAYER_HEIGHT, PlayerPos[m_nCameraNum].z);	//�����_�ݒ�

																																	//�J����POSY�̉���
		if (m_posVDest.y <= CAMERA_MIN_HIGHT)
		{
			m_posVDest.y = CAMERA_MIN_HIGHT;	//���E�l�ɖ߂�
		}

		//�ݒ�l�̔��f
		m_posV += (m_posVDest - m_posV)*0.1;
		m_posR += (m_posRDest - m_posR)*0.9;
}

//=============================================================================
//�J�����N���X�̃Z�b�g����
//=============================================================================
void CCamera::SetCamera(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//�r���[�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxView);

	//�r���[�}�g���b�N�X�̍쐬
	D3DXMatrixLookAtLH(&m_mtxView,
		&m_posV,
		&m_posR,
		&m_posU);

	//�r���[�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_VIEW, &m_mtxView);

	//�v���W�F�N�V�����}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxProjection);

	//�v���W�F�N�V�����}�g���b�N�X�̍쐬
	D3DXMatrixPerspectiveFovLH(&m_mtxProjection,
		D3DXToRadian(45.0f),
		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT / 2,
		10.0f,
		100000.0f);

	//�v���W�F�N�V�����}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_PROJECTION,
		&m_mtxProjection);
}

//=============================================================================
//�J�����̈ʒu�擾����
//=============================================================================
D3DXVECTOR3 CCamera::GetposV(void)
{
	return m_posV;
}

//=============================================================================
//�J�����̊p�x�擾����
//=============================================================================
D3DXVECTOR3 CCamera::GetposR(void)
{
	return m_posR;
}

//=============================================================================
//�J�����̃r���[�}�g���b�N�X�擾����
//=============================================================================
D3DXMATRIX CCamera::GetMtxView(void)
{
	return m_mtxView;
}

//=============================================================================
//�J�����̏c��]�p�擾����
//=============================================================================
float CCamera::Get��(void)
{
	return m_f��;
}

//=============================================================================
//�J�����̉���]�p�擾����
//=============================================================================
float CCamera::Get��(void)
{
	return m_f��;
}