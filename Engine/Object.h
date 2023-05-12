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
	/// �Q�[���I�u�W�F�N�g��R���|�[�l���g�A�A�Z�b�g�̍폜�֐�
	/// </summary>
	/// <param name="objct">�폜����I�u�W�F�N�g</param>
	/// <param name="t">�폜����܂ł̃f�B���C����</param>
	static void Destroy(Object *destroy_object, float t = 0.0f);


	/// <summary>
	/// �w�肳�ꂽ�^�Ɉ�v����I�u�W�F�N�g��Ԃ�
	/// </summary>
	/// <typeparam name="Type">���������I�u�W�F�N�g�̌^</typeparam>
	/// <returns></returns>
	template<class Type>
	static std::weak_ptr<Type> FindObjectOfType();*/

private:
	std::string name_;	// �I�u�W�F�N�g��


};

