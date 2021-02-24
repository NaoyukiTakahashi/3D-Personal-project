//=============================================================================
//
// �}�l�[�W���[���� [manager.cpp]
// Author : 
//
//=============================================================================

//=============================================================================
//�C���N���[�h�t�@�C��
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
//�ÓI�����o�ϐ��錾
//=============================================================================
CManager::MODE_TYPE CManager::m_mode = CManager::MODE_TYPE_GAME;
CRenderer *CManager::m_pRenderer = NULL;	//�����_���[�N���X�̃|�C���^�ϐ�
CInputKeyboard *CManager::m_pInput = NULL;//���͏����N���X�̃|�C���^�ϐ�
CFade *CManager::m_pFade = NULL;
CGame *CManager::m_pGame = NULL;
CScene *CManager::m_pScene = NULL;
//=============================================================================
//�R���X�g���N�^
//=============================================================================
CManager::CManager()
{
}

//=============================================================================
//�f�X�g���N�^
//=============================================================================
CManager::~CManager()
{
}

//=============================================================================
//����������
//=============================================================================
HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, bool bWindow)
{
	//�����_���[�N���X�̃C���X�^���X����
	m_pRenderer = new CRenderer;

	//���������m�ۂł�����
	if (m_pRenderer != NULL)
	{
		// ����������
		if (FAILED(m_pRenderer->Init(hWnd, bWindow)))
		{
			return -1;
		}
	}

	//���͏����N���X�̃C���X�^���X����
	m_pInput = new CInputKeyboard;

	//���������m�ۂł�����
	if (m_pInput != NULL)
	{
		if (FAILED(m_pInput->Init(hInstance, hWnd)))
		{
			return -1;
		}
	}

	//�^�C�g���N���X�̃N���G�C�g
	//m_pTitle = CTitle::Create();

	//�t�F�[�h�N���X�̃N���G�C�g
	m_pFade = CFade::Create();

	//�S�e�N�X�`���̓ǂݍ���
	LoadAll();

	return S_OK;
}

//=============================================================================
//�I������
//=============================================================================
void CManager::Uninit(void)
{
	//�S�e�N�X�`���̔j��
	UnLoadAll();

	if (m_pFade != NULL)
	{
		//�t�F�[�h�N���X�̏I�������Ăяo��
		m_pFade->Uninit();

		//�������̔j��
		delete m_pFade;

		//�������̃N���A
		m_pFade = NULL;
	}

	if (m_pInput != NULL)
	{
		//���͏����N���X�̏I�������Ăяo��
		m_pInput->Uninit();

		//�������̍폜
		delete m_pInput;

		//�������̃N���A
		m_pInput = NULL;
	}

	if (m_pRenderer != NULL)
	{
		//�����_���[�N���X�̏I�������Ăяo��
		m_pRenderer->Uninit();

		//�������̍폜
		delete m_pRenderer;

		//�������̃N���A
		m_pRenderer = NULL;
	}
}

//=============================================================================
//�X�V����
//=============================================================================
void CManager::Update(void)
{
	if (m_pInput != NULL)
	{
		//���͏����N���X�̍X�V�����Ăяo��
		m_pInput->Update();
	}

	if (m_pRenderer != NULL)
	{
		//�����_���[�N���X�̍X�V�����Ăяo��
		m_pRenderer->Update();
	}

	if (m_pFade != NULL)
	{
		//�t�F�[�h�N���X�̍X�V�����Ăяo��
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
//�`�揈��
//=============================================================================
void CManager::Draw(void)
{
	if (m_pRenderer != NULL)
	{
		//�����_���[�N���X�̕`�揈���Ăяo��
		m_pRenderer->Draw();
	}
}

//=============================================================================
//�e�N�X�`���̑S���[�h����
//=============================================================================
void CManager::LoadAll(void)
{
	CBg::Load();
}

//=============================================================================
//�e�N�X�`���̑S�A�����[�h����
//=============================================================================
void CManager::UnLoadAll(void)
{
	CBg::UnLoad();
}

//=============================================================================
//�Q�[�����[�h�̐ݒ菈��
//=============================================================================
void CManager::SetMode(MODE_TYPE mode)
{

	//���݃��[�h�̏I��
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

	//���[�h��ݒ�
	m_mode = mode;

	//�ݒ肳�ꂽ���[�h���N���G�C�g
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
//�Q�[�����[�h���̎擾
//=============================================================================
CManager::MODE_TYPE CManager::GetMode(void)
{
	return m_mode;
}

//=============================================================================
//�����_���[���擾
//=============================================================================
CRenderer * CManager::GetRenderer(void)
{
	return m_pRenderer;
}


//=============================================================================
//�L�[�{�[�h���擾
//=============================================================================
CInputKeyboard * CManager::GetKeyboard(void)
{
	return m_pInput;
}

//=============================================================================
//�t�F�[�h���擾
//=============================================================================
CFade * CManager::GetFade(void)
{
	return m_pFade;
}