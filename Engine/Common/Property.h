#pragma once
// 参考ページ

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
		/// ゲッター
		/// </summary>
		operator T() const
		{
			return getter_ ? getter_() : r_;
		}

		/// <summary>
		/// セッター
		/// </summary>
		/// <param name="v">代入したい値</param>
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
		/// アローゲッター
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
		/// 参照渡しオーバーロード
		/// </summary>
		T &operator *()const
		{
			return &r_;
		}

		T &r_;		                                           // 値
	protected:
		std::function<T()> getter_ = nullptr;                  // ゲット時の処理
		std::function<void(T value)> setter_ = nullptr;        // セット時の処理
	};

	/// <summary>
	/// 読み取り専用プロパティ
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
	/// 書き込み専用プロパティ
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
		/// セッター
		/// </summary>
		/// <param name="v">代入したい値</param>
		void operator =(const T v)
		{
			this->setter_(v);
		}
	};
}

