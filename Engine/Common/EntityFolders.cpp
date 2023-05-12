#include "EntityFolders.h"

#include <imgui.h>


namespace yEngine
{
	EntityFolders::EntityFolders()
	{
		auto folder = folders_.emplace_back(std::make_shared<Folder>());
		folder->name = "RootFolder";
	}

	std::weak_ptr<EntityFolders::Folder> EntityFolders::CreateFolder(FolderId pearentFolderID)
	{
		// IDと一致するフォルダを検索
		std::weak_ptr<Folder> pearentFolder;
		for (auto folder : folders_)
		{
			if (folder->id == pearentFolderID)
			{
				pearentFolder = folder;
				break;
			}
		}

		auto add_folder = folders_.emplace_back(std::make_shared<Folder>());	// フォルダ生成
		add_folder->parent_folder = pearentFolder;								// 親フォルダを設定

		// 指定したフォルダに子フォルダがない場合
		if (pearentFolder.lock()->child_folder.expired())
		{
			// 親フォルダ
			pearentFolder.lock()->child_folder = add_folder;	// 子に登録

		}
		else
		{
			// 指定したフォルダの子の次が空になるまで
			auto addfolder_pos = pearentFolder.lock()->child_folder.lock()->next_folder;
			while (!addfolder_pos.expired())
			{
				addfolder_pos = addfolder_pos.lock()->next_folder;
			}
			// 追加フォルダ
			add_folder->prev_folder = addfolder_pos;		// 前のフォルダに設定

			// 前フォルダ
			addfolder_pos.lock()->next_folder = add_folder;	// 次のフォルダに設定

		}

		return add_folder;
	}

	std::weak_ptr<EntityFolders::Entity> EntityFolders::CreateEntity(FolderId pearentFolderID, std::weak_ptr<Object> object)
	{
		// IDと一致するフォルダを検索
		std::weak_ptr<Folder> pearentFolder;
		for (auto folder : folders_)
		{
			if (folder->id == pearentFolderID)
			{
				pearentFolder = folder;
				break;
			}
		}

		auto add_entity = entitys_.emplace_back(std::make_shared<Entity>());	// エンティティ生成
		add_entity->pearent_folder = pearentFolder.lock()->id;					// 親フォルダを設定
		add_entity->object = object;

		// 指定したフォルダに子フォルダがない場合
		if (pearentFolder.lock()->first_entity.expired())
		{
			// 親フォルダ
			pearentFolder.lock()->first_entity = add_entity;	// 子に登録

		}
		else
		{
			// 指定したフォルダの子の次が空になるまで
			auto addentity_pos = pearentFolder.lock()->first_entity.lock()->next;
			while (!addentity_pos.expired())
			{
				addentity_pos = addentity_pos.lock()->next;
			}
			// 追加フォルダ
			add_entity->prev = addentity_pos;		// 前のフォルダに設定

			// 前フォルダ
			addentity_pos.lock()->next = add_entity;	// 次のフォルダに設定

		}

		return add_entity;

	}

}