#pragma once

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <wrl.h>

#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

#include "Common/Singleton.h"
#include "Common/Property.h"

//// �R���g���[���[�̃{�^��
//enum class GamePadButton
//{
//	INPUT_A,			//A�{�^��
//	INPUT_B,			//B�{�^��
//	INPUT_X,			//X�{�^��
//	INPUT_Y,			//Y�{�^��
//	INPUT_LB,			//LB�{�^��
//	INPUT_RB,			//RB�{�^��
//	INPUT_SELECT,		//SELECT�{�^��
//	INPUT_START,		//START�{�^��
//	INPUT_LSTICK,		//���X�e�B�b�N��������
//	INPUT_RSTICK,		//�E�X�e�B�b�N��������
//	INPUT_LEFT = 34,	//�\���L�[��
//	INPUT_RIGHT = 32,	//�\���L�[�E
//	INPUT_UP = 35,		//�\���L�[��
//	INPUT_DOWN = 33,	//�\���L�[��
//};
//// �R���g���[���[�̃X�e�B�b�N
//enum class GamePadStick
//{
//	Stick_L,
//	Stick_R,
//};
//
//enum class GamePadAxis
//{
//	AXIS_LX = 0x01,			//���X�e�B�b�NX��
//	AXIS_LY = 0x02,			//���X�e�B�b�NY��
//	AXIS_LZ = 0x04,			//���X�e�B�b�NZ��(LT�ERT)
//	AXIS_RX = 0x08,			//�E�X�e�B�b�NX��
//	AXIS_RY = 0x10,			//�E�X�e�B�b�NY��
//	AXIS_RZ = 0x20,			//�E�X�e�B�b�NZ��
//};

// ���͂̍X�V��FixedUpdate�̃^�C�~���O�ōs����
class Input final
	: public Singleton<Input>
{
public:
	friend class Singleton<Input>;

	
	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �L�[����͂��Ă���t���[������Ԃ�
	/// </summary>
	/// <returns>int ���̓t���[����</returns>
	int GetKeyPress(int key);

	/// <summary>
	/// �L�[�������ꂽ�u�Ԃ���true�ɂȂ�
	/// </summary>
	/// <returns>bool �����ꂽ�u��</returns>
	bool GetKeyPressTrigger(int key);

	/// <summary>
	/// �L�[�𗣂��Ă���t���[������Ԃ�
	/// </summary>
	/// <returns>int ���̓t���[����</returns>
	int GetKeyReleas(int key);

	/// <summary>
	/// �L�[�������ꂽ�u�Ԃ���true�ɂȂ�
	/// </summary>
	/// <returns>bool �����ꂽ�u��</returns>
	bool GetKeyReleasTrigger(int key);

	/// <summary>
	/// ���݉��炩�̃L�[���}�E�X�{�^����������Ă��邩�ǂ���
	/// </summary>
	/// <returns>������Ă����true��Ԃ�</returns>
	yEngine::ReadOnlyProperty<bool> anyKey
	{
		&any_key_
	};

	/// <summary>
	/// ���݉��炩�̃L�[���}�E�X�{�^�����������ŏ��̃t���[����Ԃ�
	/// </summary>
	/// <returns>�����ꂽ�u��true��Ԃ�</returns>
	yEngine::ReadOnlyProperty<bool> anyKeyDown
	{
		&any_key_down_
	};

private:

	Input();

	/// <summary>
	/// �L�[�{�[�h�̍X�V
	/// </summary>
	void KeyboardUpdate();

	bool any_key_;			// �����ꂩ�̓��͂�������Ă����
	bool any_key_down_;		// �����ꂩ�̓��͂������ꂽ�u��

	// �L�[�{�[�h
	Microsoft::WRL::ComPtr<IDirectInputDevice8> device_keyboard_;
	int key_press_count_[256];	// �L�[�������m�F�p
	int key_releas_count_[256];	// �L�[������m�F�p

	//// �R���g���[���[
	//static void ControllerUpdate();
	//static std::vector<IDirectInputDevice8 *> dev_gamepads_; //�Q�[���p�b�h�f�o�C�X
	//static std::vector<DIJOYSTATE> gamepads_; //�Q�[���p�b�h�̐��̏��
	//static int controller_press_count_[64];		// �R���g���[���[�������m�F�p
	//static int controller_releas_count_[64];	// �R���g���[���[������m�F�p
	//static const LONG zone_max_;					// �X�e�B�b�N���͂̏��
	//static const LONG zone_min_;					// �X�e�B�b�N���͂̉���
};

