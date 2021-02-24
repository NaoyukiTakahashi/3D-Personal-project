//=============================================================================
//
// ���C������ [input.cpp]
// Author : �R�c�ˑ�
//
//=============================================================================
#include "input.h"

//=============================================================================
//�ÓI�����o�ϐ��錾
//=============================================================================
LPDIRECTINPUT8 CInput::m_pInput = NULL;	//DirectInput�I�u�W�F�N�g�̃|�C���^

										//=============================================================================
										//���͏����N���X�̃R���X�g���N�^
										//=============================================================================
CInput::CInput()
{
	m_pDevice = NULL;
}

//=============================================================================
//���͏����N���X�̃f�X�g���N�^
//=============================================================================
CInput::~CInput()
{
}
//
//=============================================================================
//���͏����N���X�̏���������
//=============================================================================
HRESULT CInput::Init(HINSTANCE hInstance, HWND hWnd)
{
	if (m_pInput == NULL)
	{
		//DirectInput�I�u�W�F�N�g�̍쐬
		if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pInput, NULL)))
		{
			return E_FAIL;
		}
	}

	return S_OK;
}

//=============================================================================
//���͏����N���X�̏I������
//=============================================================================
void CInput::Uninit(void)
{
	//���̓f�o�C�X�̊J��
	if (m_pDevice != NULL)
	{
		//���̓f�o�C�X�ւ̃A�N�Z�X���J���i���͐���I���j
		m_pDevice->Unacquire();	//�f�o�C�X����̒�~

								//�������̍폜
		m_pDevice->Release();
		//�������̃N���A
		m_pDevice = NULL;
	}

	//DirectInput�I�u�W�F�N�g�̊J��
	if (m_pInput != NULL)
	{
		//�������̍폜
		m_pInput->Release();

		//�������̃N���A
		m_pInput = NULL;
	}
}