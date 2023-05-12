#pragma once
#include "Common.h"
#include "Object.h"

#include <memory>
#include <vector>

namespace yEngine
{
	class EntityFolders final
	{
	private:
		using FolderId = yEngine::u16;
	public:
		EntityFolders();
		

		// �G���e�B�e�B�\����
		struct Entity
		{
			FolderId pearent_folder;					// �e�̃t�H���_
			std::weak_ptr<Object> object;				// ���g
			std::weak_ptr<Entity> next;					// �t�H���_���̎��̃G���e�B�e�B
			std::weak_ptr<Entity> prev;					// �t�H���_���̑O�̃G���e�B�e�B
		};

		// �t�H���_�\����
		struct Folder
		{
			FolderId id;
			std::weak_ptr<Folder> parent_folder;		// �e�t�H���_
			std::weak_ptr<Folder> child_folder;			// �q�t�H���_
			std::weak_ptr<Folder> next_folder;			// ���t�H���_
			std::weak_ptr<Folder> prev_folder;			// �O�t�H���_
			std::weak_ptr<Entity> first_entity;			// �ŏ��̃G���e�B�e�B
			std::string name;								// �t�H���_�̖��O

		};

		std::weak_ptr<Folder> GetRoot() const {	return root_folder_;}

		std::weak_ptr<Folder> CreateFolder(FolderId pearentFolderID);
		std::weak_ptr<Entity> CreateEntity(FolderId pearentFolder,std::weak_ptr<Object> object);
		//std::weak_ptr<Folder> GetFolder(FolderId folder_id);
	private:

		std::weak_ptr<Folder> root_folder_;				// ��ԏ�̃t�H���_
		std::vector<std::shared_ptr<Folder>> folders_;					// �t�H���_���Ɋ܂܂��S�G���e�B�e�B
		std::vector<std::shared_ptr<Entity>> entitys_;					// �t�H���_���Ɋ܂܂��S�G���e�B�e�B
	};
}