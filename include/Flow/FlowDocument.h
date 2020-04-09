#include <iostream>
#include <filesystem>
#include <memory>
#include <vector>

#include <pugixml.hpp>

namespace Flow {
	class FlowDocument {
		struct Resource {};
		pugi::xml_document index;
		//std::vector<Modules> modules;
		std::vector<Resource> resources;
		std::filesystem::path tempFolder;
		std::filesystem::path filePath;
		bool spawnTempDir();
		bool destroyTempDir();
		bool _isOpen = false;
		bool pathSet = false;
		bool tempDirSpawned = false;
	public:
		FlowDocument();
		FlowDocument(std::filesystem::path const &name);
		~FlowDocument();
		//File Management
		bool create();
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