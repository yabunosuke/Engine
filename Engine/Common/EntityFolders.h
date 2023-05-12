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
		

		// エンティティ構造体
		struct Entity
		{
			FolderId pearent_folder;					// 親のフォルダ
			std::weak_ptr<Object> object;				// 中身
			std::weak_ptr<Entity> next;					// フォルダ内の次のエンティティ
			std::weak_ptr<Entity> prev;					// フォルダ内の前のエンティティ
		};

		// フォルダ構造体
		struct Folder
		{
			FolderId id;
			std::weak_ptr<Folder> parent_folder;		// 親フォルダ
			std::weak_ptr<Folder> child_folder;			// 子フォルダ
			std::weak_ptr<Folder> next_folder;			// 次フォルダ
			std::weak_ptr<Folder> prev_folder;			// 前フォルダ
			std::weak_ptr<Entity> first_entity;			// 最初のエンティティ
			std::string name;								// フォルダの名前

		};

		std::weak_ptr<Folder> GetRoot() const {	return root_folder_;}

		std::weak_ptr<Folder> CreateFolder(FolderId pearentFolderID);
		std::weak_ptr<Entity> CreateEntity(FolderId pearentFolder,std::weak_ptr<Object> object);
		//std::weak_ptr<Folder> GetFolder(FolderId folder_id);
	private:

		std::weak_ptr<Folder> root_folder_;				// 一番上のフォルダ
		std::vector<std::shared_ptr<Folder>> folders_;					// フォルダ内に含まれる全エンティティ
		std::vector<std::shared_ptr<Entity>> entitys_;					// フォルダ内に含まれる全エンティティ
	};
}