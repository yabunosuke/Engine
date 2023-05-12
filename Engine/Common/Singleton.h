#pragma once

#include <mutex>
#include <assert.h>

class SingletonFinalizer
{
public:
	using FinalizerFunc = void(*)();
	static void AddFinalizer(FinalizerFunc func);
	static void Finalize();
};

template <typename T>
class Singleton
{
public:
	/// <summary>
	/// �C���X�^���X�擾
	/// </summary>
	/// <returns></returns>
	static T &GetInstance()
	{
		return *instance_;
	}

	/// <summary>
	/// ����
	/// </summary>
	static void CreateSingleton()
	{
		if (instance_ == nullptr)
		{
			instance_ = new T;
			SingletonFinalizer::AddFinalizer(&Singleton<T>::Destroy);
			if (!instance_->StartUp())
			{
				assert(0);
			}
		}
		else
		{

		}
	}

protected:
	Singleton() = default;			// �O������̐����֎~
	virtual ~Singleton() = default;	// �O������̔j���֎~

	virtual bool StartUp() { return true; };	// �R���X�g���N�^�̌�Ɏ��s��������

private:
	void operator=(const Singleton &singleton) {};	// ������Z�֎~
	//Singlton(const Singleton &singleton) {};			// �R�s�[�R���X�g���N�^�֎~



	/// <summary>
	/// �j��
	/// </summary>
	static void Destroy()
	{
		delete instance_;
		instance_ = nullptr;
	}

	static T *instance_;				// �C���X�^���X
};

template <typename T> T *Singleton<T>::instance_ = nullptr;