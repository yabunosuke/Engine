#pragma once

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <wrl.h>

#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

#include "Common/Singleton.h"
#include "Common/Property.h"

//// コントローラーのボタン
//enum class GamePadButton
//{
//	INPUT_A,			//Aボタン
//	INPUT_B,			//Bボタン
//	INPUT_X,			//Xボタン
//	INPUT_Y,			//Yボタン
//	INPUT_LB,			//LBボタン
//	INPUT_RB,			//RBボタン
//	INPUT_SELECT,		//SELECTボタン
//	INPUT_START,		//STARTボタン
//	INPUT_LSTICK,		//左スティック押し込み
//	INPUT_RSTICK,		//右スティック押し込み
//	INPUT_LEFT = 34,	//十字キー左
//	INPUT_RIGHT = 32,	//十字キー右
//	INPUT_UP = 35,		//十字キー上
//	INPUT_DOWN = 33,	//十字キー下
//};
//// コントローラーのスティック
//enum class GamePadStick
//{
//	Stick_L,
//	Stick_R,
//};
//
//enum class GamePadAxis
//{
//	AXIS_LX = 0x01,			//左スティックX軸
//	AXIS_LY = 0x02,			//左スティックY軸
//	AXIS_LZ = 0x04,			//左スティックZ軸(LT・RT)
//	AXIS_RX = 0x08,			//右スティックX軸
//	AXIS_RY = 0x10,			//右スティックY軸
//	AXIS_RZ = 0x20,			//右スティックZ軸
//};

// 入力の更新はFixedUpdateのタイミングで行われる
class Input final
	: public Singleton<Input>
{
public:
	friend class Singleton<Input>;

	
	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// キーを入力しているフレーム数を返す
	/// </summary>
	/// <returns>int 入力フレーム数</returns>
	int GetKeyPress(int key);

	/// <summary>
	/// キーが押された瞬間だけtrueになる
	/// </summary>
	/// <returns>bool 押された瞬間</returns>
	bool GetKeyPressTrigger(int key);

	/// <summary>
	/// キーを離しているフレーム数を返す
	/// </summary>
	/// <returns>int 入力フレーム数</returns>
	int GetKeyReleas(int key);

	/// <summary>
	/// キーが離された瞬間だけtrueになる
	/// </summary>
	/// <returns>bool 押された瞬間</returns>
	bool GetKeyReleasTrigger(int key);

	/// <summary>
	/// 現在何らかのキーかマウスボタンが押されているかどうか
	/// </summary>
	/// <returns>押されている間trueを返す</returns>
	yEngine::ReadOnlyProperty<bool> anyKey
	{
		&any_key_
	};

	/// <summary>
	/// 現在何らかのキーかマウスボタンを押した最初のフレームを返す
	/// </summary>
	/// <returns>押された瞬間trueを返す</returns>
	yEngine::ReadOnlyProperty<bool> anyKeyDown
	{
		&any_key_down_
	};

private:

	Input();

	/// <summary>
	/// キーボードの更新
	/// </summary>
	void KeyboardUpdate();

	bool any_key_;			// いずれかの入力が押されている間
	bool any_key_down_;		// いずれかの入力が押された瞬間

	// キーボード
	Microsoft::WRL::ComPtr<IDirectInputDevice8> device_keyboard_;
	int key_press_count_[256];	// キー長押し確認用
	int key_releas_count_[256];	// キー手放し確認用

	//// コントローラー
	//static void ControllerUpdate();
	//static std::vector<IDirectInputDevice8 *> dev_gamepads_; //ゲームパッドデバイス
	//static std::vector<DIJOYSTATE> gamepads_; //ゲームパッドの生の状態
	//static int controller_press_count_[64];		// コントローラー長押し確認用
	//static int controller_releas_count_[64];	// コントローラー手放し確認用
	//static const LONG zone_max_;					// スティック入力の上限
	//static const LONG zone_min_;					// スティック入力の下限
};

