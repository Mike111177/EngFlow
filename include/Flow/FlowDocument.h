#pragma once
#include <iostream>
#include <filesystem>
#include <memory>
#include <vector>

#include <Flow/Block.h>

namespace Flow {
	class FlowDocument {
		//std::vector<Modules> modules;
		std::filesystem::path filePath;
		/*std::filesystem::path tempFolder;
		bool spawnTempDir();
		bool destroyTempDir();
		bool tempDirSpawned = false;*/
		bool _isOpen = false;
		bool pathSet = false;
	public:
		std::vector<std::shared_ptr<Block>> globalBlocks;
		FlowDocument();
		FlowDocument(std::filesystem::path const &name);
		~FlowDocument();
		//File Management
		bool open();
		bool open(std::filesystem::path const& name);
		bool isOpen();
		void setName(std::filesystem::path const& name);
		bool save();
		bool save(std::filesystem::path const& name);
		bool saveAs(std::filesystem::path const& name);
		void close();
		//Data operations
	};

}