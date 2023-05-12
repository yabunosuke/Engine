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
		// ID�ƈ�v����t�H���_������
		std::weak_ptr<Folder> pearentFolder;
		for (auto folder : folders_)
		{
			if (folder->id == pearentFolderID)
			{
				pearentFolder = folder;
				break;
			}
		}

		auto add_folder = folders_.emplace_back(std::make_shared<Folder>());	// �t�H���_����
		add_folder->parent_folder = pearentFolder;								// �e�t�H���_��ݒ�

		// �w�肵���t�H���_�Ɏq�t�H���_���Ȃ��ꍇ
		if (pearentFolder.lock()->child_folder.expired())
		{
			// �e�t�H���_
			pearentFolder.lock()->child_folder = add_folder;	// �q�ɓo�^

		}
		else
		{
			// �w�肵���t�H���_�̎q�̎�����ɂȂ�܂�
			auto addfolder_pos = pearentFolder.lock()->child_folder.lock()->next_folder;
			while (!addfolder_pos.expired())
			{
				addfolder_pos = addfolder_pos.lock()->next_folder;
			}
			// �ǉ��t�H���_
			add_folder->prev_folder = addfolder_pos;		// �O�̃t�H���_�ɐݒ�

			// �O�t�H���_
			addfolder_pos.lock()->next_folder = add_folder;	// ���̃t�H���_�ɐݒ�

		}

		return add_folder;
	}

	std::weak_ptr<EntityFolders::Entity> EntityFolders::CreateEntity(FolderId pearentFolderID, std::weak_ptr<Object> object)
	{
		// ID�ƈ�v����t�H���_������
		std::weak_ptr<Folder> pearentFolder;
		for (auto folder : folders_)
		{
			if (folder->id == pearentFolderID)
			{
				pearentFolder = folder;
				break;
			}
		}

		auto add_entity = entitys_.emplace_back(std::make_shared<Entity>());	// �G���e�B�e�B����
		add_entity->pearent_folder = pearentFolder.lock()->id;					// �e�t�H���_��ݒ�
		add_entity->object = object;

		// �w�肵���t�H���_�Ɏq�t�H���_���Ȃ��ꍇ
		if (pearentFolder.lock()->first_entity.expired())
		{
			// �e�t�H���_
			pearentFolder.lock()->first_entity = add_entity;	// �q�ɓo�^

		}
		else
		{
			// �w�肵���t�H���_�̎q�̎�����ɂȂ�܂�
			auto addentity_pos = pearentFolder.lock()->first_entity.lock()->next;
			while (!addentity_pos.expired())
			{
				addentity_pos = addentity_pos.lock()->next;
			}
			// �ǉ��t�H���_
			add_entity->prev = addentity_pos;		// �O�̃t�H���_�ɐݒ�

			// �O�t�H���_
			addentity_pos.lock()->next = add_entity;	// ���̃t�H���_�ɐݒ�

		}

		return add_entity;

	}

}