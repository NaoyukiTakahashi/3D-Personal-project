//=======================================================================================
//
// �Q�[������ [game.cpp]
// Author : Konishi Yuuto
//
//=======================================================================================

//=======================================================================================
// �C���N���[�h
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
// static������
//=======================================================================================
CCamera *CGame::m_apCamera[MAX_PLAYER] = {};		// �J�����N���X�̃|�C���^�ϐ�
CLight *CGame::m_pLight = NULL;						// ���C�g�N���X�̃|�C���^�ϐ�
CMeshField *CGame::m_pMeshField = NULL;				// ���b�V���t�B�[���h
CMeshShape *CGame::m_pSphere = NULL;				// ���b�V���X�t�B�A
CBg *CGame::m_pBg = NULL;							// �w�i�̃|�C���^
CPlayer *CGame::m_apPlayer[MAX_PLAYER] = {};		// �v���C���[�̃|�C���^
CTime *CGame::m_pTime = NULL;						// �^�C���̃|�C���^
CUi *CGame::m_pUi = NULL;							// ui�̃|�C���^
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
// �R���X�g���N�^
//=======================================================================================
CGame::CGame(int nPriority) : CScene(nPriority)
{
	m_bGameEnd = false;
}

//=======================================================================================
// �f�X�g���N�^
//=======================================================================================
CGame::~CGame()
{
	m_nRoundNum = 0;
}

//=======================================================================================
// �N���G�C�g
//=======================================================================================
CGame* CGame::Create(void)
{
	// �������m��
	CGame* pGame = new CGame();

	// ����������
	pGame->Init(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	return pGame;
}

//=======================================================================================
// ����������
//=======================================================================================
HRESULT CGame::Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 size)
{
	m_nRoundNum = 0;

	CInputKeyboard *pKeyboard = CManager::GetKeyboard();

	for (int nCount = 0; nCount < MAX_PLAYER; nCount++)
	{
		//�J�����N���X�̃N���G�C�g
		m_apCamera[nCount] = CCamera::Create();
	}

	//���C�g�N���X�̐���
	m_pLight = new CLight;

	// ���C�g�̏���������
	if (m_pLight != NULL)
	{
		if (FAILED(m_pLight->Init()))
		{
			return -1;
		}
	}

	// �v���C���[�̐���
	if (m_apPlayer[0] == NULL)
	{
		m_apPlayer[0] = CPlayer::Create(D3DXVECTOR3(0.0f, 0.0f, -1000.0f), D3DXVECTOR3(PLAYER_SIZE_X, PLAYER_SIZE_Y, PLAYER_SIZE_Z));
	}
	if (m_apPlayer[1] == NULL)
	{
		m_apPlayer[1] = CPlayer::Create(D3DXVECTOR3(0.0f, 0.0f, 1000.0f), D3DXVECTOR3(PLAYER_SIZE_X, PLAYER_SIZE_Y, PLAYER_SIZE_Z));
	}

	// �w�i
	if (m_pBg == NULL)
	{
		m_pBg = CBg::Create();
	}

	//�f�o�C�X���̎擾
	LPDIRECT3DDEVICE9 pD3DDevice = CManager::GetRenderer()->GetDevice();

	//�t�H���g�̐���
	D3DXCreateFont(pD3DDevice, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Terminal", &m_pFont);

	return S_OK;
}

//=======================================================================================
// �I������
//=======================================================================================
void CGame::Uninit(void)
{
	for (int nCount = 0; nCount < MAX_PLAYER; nCount++)
	{
		if (m_apCamera[nCount] != NULL)
		{
			//�J�����N���X�̏I�������Ăяo��
			m_apCamera[nCount]->Uninit();

			//�������̔j��
			delete m_apCamera[nCount];

			//�������̃N���A
			m_apCamera[nCount] = NULL;
		}
	}

	//CMeshShape::UnLoad();
	//// ���b�V���X�t�B�A
	//if (m_pSphere != NULL)
	//{
	//	m_pSphere->Uninit();
	//}

	// �w�i
	if (m_pBg != NULL)
	{
		m_pBg->Uninit();
		m_pBg = NULL;
	}

	// ���C�g�̏I������
	if (m_pLight != NULL)
	{
		m_pLight->Uninit();
		delete m_pLight;
		m_pLight = NULL;
	}

	// �v���C���[�I������
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
// �X�V����
//=======================================================================================
void CGame::Update(void)
{
	for (int nCount = 0; nCount < MAX_PLAYER; nCount++)
	{
		if (m_apCamera[nCount] != NULL)
		{
			//�J�����N���X�̍X�V����

			m_apCamera[nCount]->Update();
		}
	}
}

//=======================================================================================
// �`�揈��
//=======================================================================================
void CGame::Draw(void)
{
	// �w�i
	if (m_pBg != NULL)
	{
		m_pBg->Draw();
	}
}

//=======================================================================================
// �J�����̏��
//=======================================================================================
CCamera * CGame::GetCamera(int nCount)
{
	return m_apCamera[nCount];
}

//=======================================================================================
// ���C�g�̏��
//=======================================================================================
CLight * CGame::GetLight(void)
{
	return m_pLight;
}

//=======================================================================================
// �v���C���[�̏��
//=======================================================================================
CPlayer * CGame::GetPlayer(int nCount)
{
	return m_apPlayer[nCount];
}

//=======================================================================================
// �^�C���̏��
//=======================================================================================
CTime * CGame::GetTime(void)
{
	return m_pTime;
}

//=======================================================================================
// ui�̏��
//=======================================================================================
CUi * CGame::GetUi(void)
{
	return m_pUi;
}


//=======================================================================================
// uistart�̏��
//=======================================================================================
CUiStart * CGame::GetUiStart(void)
{
	return m_pUiStart;
}

//=======================================================================================
// uiend�̏��
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
// ���b�N�I���̏��
//=======================================================================================
CLockon * CGame::GetLockon(void)
{

	return m_pLockon;
}

//=======================================================================================
// �r���̏��
//=======================================================================================
CBill * CGame::GetBill(void)
{
	return m_pBill;
}

//=======================================================================================
// �R���e�j���[�̏��
//=======================================================================================
CContinue * CGame::GetContinue(void)
{
	return m_pContinue;
}

//=======================================================================================
// �R���e�j���[�̏��
//=======================================================================================
CPause * CGame::GetPause(void)
{
	return m_pPause;
}
