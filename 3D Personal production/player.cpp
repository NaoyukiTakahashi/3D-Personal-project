//=============================================================================
//
// �v���C���[�N���X [player.cpp]
// Author : Konishi Yuuto
//
//=============================================================================

//=============================================================================
// �C���N���[�h
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
// �}�N����`
//=============================================================================
#define PLAYER_SPEED			(20.0f)				// �v���C���[�̈ړ���
#define PLAYER_DUSH				(37.5f)				// �v���C���[�̃_�b�V��
#define PLAYER_DUSH_INTER		(80)				// �_�b�V�����ł��钷��
#define DUSH_NONE_TIME			(50)				// �_�b�V���ł��Ȃ�����
#define PLAYER_JUMP				(10.0f)				// �W�����v�̏���
#define GRAVITY_POWAR			(0.15f)				// �d�͂̋���
#define PLAYER_FALL				(-13.0f)			// �}�~���̏���
#define GROUND_RIMIT			(0.0f)				// �n�ʂ̐���
#define PLAYE_ROT_Y_FRONT		(D3DXToRadian(90.0f))	//�v���C���[�̏c���O
#define PLAYE_ROT_Y_LEFT		(D3DXToRadian(180.0f))	//�v���C���[�̏c����
#define PLAYE_ROT_Y_RIGHT		(D3DXToRadian(0.0f))	//�v���C���[�̏c���E
#define PLAYE_ROT_Y_BUCK		(D3DXToRadian(-90.0f))	//�v���C���[�̏c����
#define STICK_SENSITIVITY		(50.0f)					//�X�e�B�b�N���x
#define STATE_DAMAGE_TIME		(100)				// �_���[�W��Ԃ̃J�E���g
#define STATE_EXPLOSION_TIME	(30)				// ������Ԃ̃J�E���g
#define STATE_EXPLOSION_END		(500)				// ������Ԃ̏I���t���[��
#define PLAYER_BOMB_INTER		(150)				// �{���̑ҋ@�t���[��
#define PLAYER_MISSILE			(150)				// �~�T�C���̑ҋ@�t���[��
#define PLAYER_LASER_INTER		(300)				// ���[�U�[�̑ҋ@�t���[��

#define TEST_XFAILE_NAME "data/Text/Test.txt"	//LBX�̃t�@�C���p�X

#define LBX_XFAILE_NAME "data/Text/motion_LBX.txt"	//LBX�̃t�@�C���p�X
#define GANDAMU_XFAILE_NAME "data/Text/motion_gandamu.txt"	//�K���_���̃t�@�C���p�X
//=============================================================================
// �O���[�o���ϐ��錾
//=============================================================================
MODELFILLE g_modelfile[MAX_MODEL_PARTS];	//���f���p�[�c���.

//=============================================================================
// static������
//=============================================================================
int CPlayer::m_nPlayerAll = 0;				// �v���C���[�̑���

//=============================================================================
// �N���G�C�g
//=============================================================================
CPlayer * CPlayer::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	// ����������
	CPlayer *pPlayer = new CPlayer;

	// �v���C���[�̔ԍ�����
	pPlayer->m_nPlayerNum = m_nPlayerAll++;

	// ����������
	pPlayer->Init(pos, size);

	return pPlayer;
}

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CPlayer::CPlayer(int nPriority) : CScene(nPriority)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_OldPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nPlayerNum = 0;						// �v���C���[�̔ԍ�
	m_fAngle = 0.0f;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CPlayer::~CPlayer()
{
	m_nPlayerAll = 0;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CPlayer::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_OldPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fAngle = 0.0f;

	//�ʒu�̐ݒ�
	m_pos = pos;

	//ROT�̏����l�ݒ�i�G�̕����j
	m_rotDest = D3DXVECTOR3(0.0f, D3DXToRadian(0.0f), 0.0f);

	//�t�@�C���ǂݍ���
	if (FAILED(ReadFile()))
	{
		return E_FAIL;
	}

	//�I�u�W�F�N�g�^�C�v�̐ݒ�
	SetObjType(CScene::OBJTYPE_PLAYER);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CPlayer::Uninit(void)
{
	//�I�u�W�F�N�g�̔j��
	SetDeathFlag();
}

//=============================================================================
// �X�V����
//=============================================================================
void CPlayer::Update(void)
{
	// �L�[�{�[�h�X�V
	CInputKeyboard *pKeyboard = CManager::GetKeyboard();

	// �Â����W�擾
	m_OldPos = m_pos;

	// �d�͂�������
	m_move.y -= GRAVITY_POWAR;
	m_pos.y += m_move.y;		// ����

	// �n�ʂ̐���
	GroundLimit();

	//�ʒu�ֈړ��ʂ����Z
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

	// W�L�[���������Ƃ�
	if (pKeyboard->GetPress(DIK_W))
	{
		m_rotDest.y = D3DXToDegree(m_fAngle);
		m_pos.z += cosf(0)*PLAYER_SPEED;
	}

	// S�L�[���������Ƃ�
	if (pKeyboard->GetPress(DIK_S))
	{

		m_rotDest.y = sin(m_fAngle);
		m_pos.z += cosf(D3DX_PI)*PLAYER_SPEED;
	}
	// A�L�[���������Ƃ�
	if (pKeyboard->GetPress(DIK_A))
	{
		m_pos.x += sinf(-D3DX_PI / 2)*PLAYER_SPEED;
	}
	// D�L�[���������Ƃ�
	if (pKeyboard->GetPress(DIK_D))
	{
		m_pos.x += sinf(D3DX_PI / 2)*PLAYER_SPEED;
	}

	//�͈͊O�ɍs���Ȃ��悤�ɂ��鏈��
	MapLimit();

}

//=============================================================================
// �`�揈��
//=============================================================================
void CPlayer::Draw(void)
{
	// �`�揈��
	//�f�o�C�X���̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans, mtxScale;

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);					// ���ʂ��i�����j���J�����O

	for (int nCntModelNum = 0; nCntModelNum < MAX_MODEL_PARTS; nCntModelNum++)
	{
		//if (m_apModelAnime[nCntModelNum] != NULL)
		//{

		//	//�K�w���f���N���X�̕`�揈��
		//	m_apModelAnime[nCntModelNum]->Draw();
		//}
	}

	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);					// ���ʂ��i�����j���J�����O
}

//=============================================================================
// �n�ʂ̐���
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
// �͈͊O�ɍs���Ȃ��悤�ɂ��鏈��
//=============================================================================

void CPlayer::MapLimit(void)
{
	//�E
	if (m_pos.x > MAP_LIMIT)
	{
		m_pos.x = MAP_LIMIT;
	}

	//��
	if (m_pos.x <-MAP_LIMIT)
	{
		m_pos.x = -MAP_LIMIT;
	}

	//��
	if (m_pos.z > MAP_LIMIT)
	{
		m_pos.z = MAP_LIMIT;
	}

	//��O
	if (m_pos.z <-MAP_LIMIT)
	{
		m_pos.z = -MAP_LIMIT;
	}
}

//=============================================================================
// �t�@�C���ǂݍ��ݏ���
//=============================================================================
HRESULT CPlayer::ReadFile(void)
{
	
	//D3DXLoadMeshHierarchyFromX(TEST_XFAILE_NAME,);

	//FILE *pFile = NULL;	//FILE�|�C���^
	//char aHeadData[1024];
	//char aModeName[1024];
	//int nModelIndex = 0;	//���f���̃C���f�b�N�X
	//int nMotionType = 0;	//���[�V�����̃^�C�v
	//int nKeyNum = 0;		//�L�[�ԍ�
	//int nMotionNum = 0;		//���[�V�����ԍ�
	//if (m_nPlayerNum == 0)
	//{
	//	//�t�@�C���I�[�v��
	//	pFile = fopen(LBX_XFAILE_NAME, "r");
	//}
	//else
	//{
	//	//�t�@�C���I�[�v��
	//	pFile = fopen(GANDAMU_XFAILE_NAME, "r");
	//}

	//if (pFile != NULL)
	//{
	//	do
	//	{
	//		//���ǂݍ���Ń��[�h���𒊏o
	//		fgets(aHeadData, sizeof(aHeadData), pFile);
	//		sscanf(aHeadData, "%s", aModeName);

	//		if (strcmp(aModeName, "MODEL_FILENAME") == 0)
	//		{
	//			//X�t�@�C���̖��O
	//			sscanf(aHeadData, "%*s %*s %s %*s %*s", g_modelfile[nModelIndex].xFileName);

	//			//�C���f�b�N�X���P�i�߂�
	//			nModelIndex++;
	//		}

	//		if (strcmp(aModeName, "CHARACTERSET") == 0)
	//		{
	//			//�C���f�b�N�X���ŏ��ɖ߂�
	//			nModelIndex = 0;

	//			//END_MOTIONSET��ǂݍ��ނ܂ŌJ��Ԃ�
	//			while (strcmp(aModeName, "END_CHARACTERSET") != 0)
	//			{
	//				//���ǂݍ���Ń��[�h���𒊏o
	//				fgets(aHeadData, sizeof(aHeadData), pFile);
	//				sscanf(aHeadData, "%s", aModeName);

	//				if (strcmp(aModeName, "PARTSSET") == 0)
	//				{
	//					//END_PARTSSET��ǂݍ��ނ܂ŌJ��Ԃ�
	//					while (strcmp(aModeName, "END_PARTSSET") != 0)
	//					{
	//						//���ǂݍ���Ń��[�h���𒊏o
	//						fgets(aHeadData, sizeof(aHeadData), pFile);
	//						sscanf(aHeadData, "%s", aModeName);

	//						if (strcmp(aModeName, "PARENT") == 0)
	//						{
	//							//�e�q���̐ݒ�
	//							sscanf(aHeadData, "%*s %*s %d", &g_modelfile[nModelIndex].nParent);
	//						}
	//						if (strcmp(aModeName, "POS") == 0)
	//						{
	//							//�ʒu�̐ݒ�
	//							sscanf(aHeadData, "%*s %*s %f %f %f", &g_modelfile[nModelIndex].offsetPos.x,
	//								&g_modelfile[nModelIndex].offsetPos.y, &g_modelfile[nModelIndex].offsetPos.z);
	//						}
	//						if (strcmp(aModeName, "ROT") == 0)
	//						{
	//							//�����̐ݒ�
	//							sscanf(aHeadData, "%*s %*s %f %f %f", &g_modelfile[nModelIndex].offsetRot.x,
	//								&g_modelfile[nModelIndex].offsetRot.y, &g_modelfile[nModelIndex].offsetRot.z);
	//						}
	//					}
	//					//�C���f�b�N�X���P�i�߂�
	//					nModelIndex++;
	//				}
	//			}
	//		}

	//		//���[�V�����Z�b�g
	//		if (strcmp(aModeName, "MOTIONSET") == 0)
	//		{
	//			//END_MOTIONSET��ǂݍ��ނ܂ŌJ��Ԃ�
	//			while (strcmp(aModeName, "END_MOTIONSET") != 0)
	//			{
	//				//���ǂݍ���Ń��[�h���𒊏o
	//				fgets(aHeadData, sizeof(aHeadData), pFile);
	//				sscanf(aHeadData, "%s", aModeName);

	//				//���[�v���̎擾
	//				if (strcmp(aModeName, "LOOP") == 0)
	//				{
	//					sscanf(aHeadData, "%*s %*s %d", (int*)&m_Motion[nMotionType].bLoop);
	//				}

	//				//�L�[���̎擾
	//				if (strcmp(aModeName, "NUM_KEY") == 0)
	//				{
	//					sscanf(aHeadData, "%*s %*s %d", (int*)&m_Motion[nMotionType].nNumKey);
	//				}

	//				if (strcmp(aModeName, "KEYSET") == 0)
	//				{
	//					//END_KEYSET�ɂȂ�܂ŌJ��Ԃ�
	//					while (strcmp(aModeName, "END_KEYSET") != 0)
	//					{
	//						//���ǂݍ���Ń��[�h���𒊏o
	//						fgets(aHeadData, sizeof(aHeadData), pFile);
	//						sscanf(aHeadData, "%s", aModeName);

	//						//�t���[�����̎擾
	//						if (strcmp(aModeName, "FRAME") == 0)
	//						{
	//							sscanf(aHeadData, "%*s %*s %d", &m_Motion[nMotionType].aKeyInfo[nMotionNum].nFrame);
	//						}

	//						//�e�L�[�̃I�t�Z�b�g���̎擾
	//						if (strcmp(aModeName, "KEY") == 0)
	//						{
	//							//END_KEY�ɂȂ�܂ŌJ��Ԃ�
	//							while (strcmp(aModeName, "END_KEY") != 0)
	//							{
	//								//���ǂݍ���Ń��[�h���𒊏o
	//								fgets(aHeadData, sizeof(aHeadData), pFile);
	//								sscanf(aHeadData, "%s", aModeName);

	//								//�ʒu�̎擾
	//								if (strcmp(aModeName, "POS") == 0)
	//								{
	//									sscanf(aHeadData, "%*s %*s %f %f %f",
	//										&m_Motion[nMotionType].aKeyInfo[nMotionNum].aKey[nKeyNum].fPosX,
	//										&m_Motion[nMotionType].aKeyInfo[nMotionNum].aKey[nKeyNum].fPosY,
	//										&m_Motion[nMotionType].aKeyInfo[nMotionNum].aKey[nKeyNum].fPosZ);
	//								}

	//								//�����̎擾
	//								if (strcmp(aModeName, "ROT") == 0)
	//								{
	//									sscanf(aHeadData, "%*s %*s %f %f %f",
	//										&m_Motion[nMotionType].aKeyInfo[nMotionNum].aKey[nKeyNum].fRotX,
	//										&m_Motion[nMotionType].aKeyInfo[nMotionNum].aKey[nKeyNum].fRotY,
	//										&m_Motion[nMotionType].aKeyInfo[nMotionNum].aKey[nKeyNum].fRotZ);
	//								}
	//							}
	//							//�ǂݏI�������J�E���g��1�i�߂�
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

	//	//�t�@�C���N���[�Y
	//	fclose(pFile);

	//	return S_OK;
	//}
	//else
	//{
		//���s�����ꍇ���b�Z�[�W�{�b�N�X��\��
		MessageBox(NULL, "���[�V�����t�@�C�����J���̂Ɏ��s���܂���", "�x��", MB_OK | MB_ICONEXCLAMATION);

		return	E_FAIL;
	//}
}

void CPlayer::SetPos(D3DXVECTOR3 pos)
{
	m_pos = pos;
}

//=============================================================================
// ���W�̏��
//=============================================================================
D3DXVECTOR3 CPlayer::GetPos(void)
{
	return m_pos;
}

//=============================================================================
// �Â����W���
//=============================================================================
D3DXVECTOR3 CPlayer::GetOldPos(void)
{
	return m_OldPos;
}

//=============================================================================
// �ړ��ʂ̐ݒ�
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