#pragma once

#include <memory>
#include <string>

#include <cereal/types/base_class.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/list.hpp>

class Object :
	public std::enable_shared_from_this<Object>

{
public:

	/*Object(const std::string &name = "Object");
	virtual ~Object();


	/// <summary>
	/// ゲームオブジェクトやコンポーネント、アセットの削除関数
	/// </summary>
	/// <param name="objct">削除するオブジェクト</param>
	/// <param name="t">削除するまでのディレイ時間</param>
	static void Destroy(Object *destroy_object, float t = 0.0f);


	/// <summary>
	/// 指定された型に一致するオブジェクトを返す
	/// </summary>
	/// <typeparam name="Type">見つけたいオブジェクトの型</typeparam>
	/// <returns></returns>
	template<class Type>
	static std::weak_ptr<Type> FindObjectOfType();*/

private:
	std::string name_;	// オブジェクト名


};

