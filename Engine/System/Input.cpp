#include "Input.h"

#include "System/Screen.h"
#include <vector>



Input::Input()
{
	HRESULT result;
	IDirectInput8 *dinput = nullptr;

	result = DirectInput8Create(
		Screen::GetInstance().WndClass->hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void **)&dinput, nullptr);

	// �L�[�{�[�h������
	{
		result = dinput->CreateDevice(GUID_SysKeyboard, &device_keyboard_, NULL);
		result = device_keyboard_->SetDataFormat(&c_dfDIKeyboard);   //�W���`��
		result = device_keyboard_->SetCooperativeLevel(
			Screen::GetInstance().Hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	}


}

void Input::KeyboardUpdate()
{

	HRESULT result;

	result = device_keyboard_->Acquire();
	BYTE key[256] = {};
	result = device_keyboard_->GetDeviceState(sizeof(key), key);

	// �S�L�[���`�F�b�N
	for (int i = 0; i < 256; i++)
	{

		if (key[i] != 0)
		{
			if (key_releas_count_[i] != 0)
			{
				key_releas_count_[i] = 0;
			}
			key_press_count_[i]++;

			// �L�[���͂�����̂�true�ɕύX
			any_key_ = true;
		}
		else
		{
			if (key_press_count_[i] != 0)
			{
				key_press_count_[i] = 0;
			}
			key_releas_count_[i]++;
		}
	}
}


void Input::Update()
{
	any_key_ = false;	// ���̓t���O������


	KeyboardUpdate();	// �L�[�{�[�h�X�V

	//// �R���g���[���[�X�V
	//ControllerUpdate();
}

int Input::GetKeyPress(int key)
{
	return key_press_count_[key];
}

bool Input::GetKeyPressTrigger(int key)
{
	return key_press_count_[key] == 1;
}

int Input::GetKeyReleas(int key)
{
	return key_releas_count_[key];
}

bool Input::GetKeyReleasTrigger(int key)
{
	return key_releas_count_[key] == 1;
}