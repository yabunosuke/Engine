#pragma once
// �Q�l�y�[�W

#include <functional>
#include <stdexcept>

namespace yEngine
{
	template<class T>
	class Property
	{
	public:

		Property(
			T *r = nullptr,
			std::function<T()> get = nullptr,
			std::function<void(T value)> set = nullptr) :
			r_(*r),
			getter_(get),
			setter_(set)
		{
		}
		/// <summary>
		/// �Q�b�^�[
		/// </summary>
		operator T() const
		{
			return getter_ ? getter_() : r_;
		}

		/// <summary>
		/// �Z�b�^�[
		/// </summary>
		/// <param name="v">����������l</param>
		void operator =(const T v)
		{
			if (setter_)
			{
				this->setter_(v);
			}
			else
			{
				r_ = v;
			}
		}

		/// <summary>
		/// �A���[�Q�b�^�[
		/// </summary>
		T *operator ->() const
		{
			if (getter_)
			{
				return &this->getter_();
			}
			else
			{
				return &this->r_;
			}
		}

		/// <summary>
		/// �Q�Ɠn���I�[�o�[���[�h
		/// </summary>
		T &operator *()const
		{
			return &r_;
		}

		T &r_;		                                           // �l
	protected:
		std::function<T()> getter_ = nullptr;                  // �Q�b�g���̏���
		std::function<void(T value)> setter_ = nullptr;        // �Z�b�g���̏���
	};

	/// <summary>
	/// �ǂݎ���p�v���p�e�B
	/// </summary>
	/// <typeparam name="T"></typeparam>
	template<class T>
	class ReadOnlyProperty :
		public Property<T>
	{
	public:
		ReadOnlyProperty(
			T *r,
			std::function<T()> get = nullptr ) :
			Property<T> (
				r,
				get,
				[](T) { throw std::runtime_error("This is ReedOnly property."); })
		{
		}

	};


	/// <summary>
	/// �������ݐ�p�v���p�e�B
	/// </summary>
	/// <typeparam name="T"></typeparam>
	template<class T>
	class WriteOnlyProperty :
		public Property<T>
	{
	public:
		WriteOnlyProperty(
			std::function<void(T)> setter) :
			Property<T>(
				[]() -> T { throw std::runtime_error("This is WriteOnly property."); },
				setter)
		{
		}

		/// <summary>
		/// �Z�b�^�[
		/// </summary>
		/// <param name="v">����������l</param>
		void operator =(const T v)
		{
			this->setter_(v);
		}
	};
}

