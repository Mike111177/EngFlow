#include <stdexcept>
#include <sstream>
#include <random>

#include <pugixml.hpp>
#include <zip.h>

#include <Flow/FlowDocument.h>

Flow::FlowDocument::FlowDocument() {}

Flow::FlowDocument::FlowDocument(std::filesystem::path const &name): FlowDocument() {setName(name);}

Flow::FlowDocument::~FlowDocument() { close(); }

bool Flow::FlowDocument::open() {
	close();
	if (!pathSet) throw std::runtime_error("Document has no path given to load from");
	int err = 0;
	//Open Zip Folder
	zip_t* zf = zip_open(filePath.string().c_str(), ZIP_RDONLY, &err);
	if (err) {
		zip_error_t zerr;
		zip_error_init_with_code(&zerr, err);
		std::cerr << "Could not open file: " << filePath << std::endl 
			      << "\tReason: " << zip_error_strerror(&zerr) << std::endl;
		return _isOpen = false;
	}
	
	//Find and load index.xml
	zip_stat_t sb;
	zip_stat_init(&sb);
	zip_stat(zf, "index.xml", 0, &sb);
	auto indexFile = zip_fopen_index(zf, sb.index, 0);
	std::unique_ptr<char[]> indexBuf(new char[sb.size]);
	zip_fread(indexFile, indexBuf.get(), sb.size);
	pugi::xml_document index;
	index.load_buffer(indexBuf.get(), sb.size);

	auto globalBlocksX = index.select_node("FlowIndex/GlobalBlocks");
	if (globalBlocksX) {
		for (auto& blockX : globalBlocksX.node().children()) {
			std::string name = blockX.attribute("name").value();
			std::string type = blockX.attribute("type").value();
			auto blockOpt = Flow::Block::create(type);
			if (blockOpt) {
				auto block = blockOpt.value();
				FlowResourceList resources;
				for (auto& resX : blockX.children()) {
					std::string resName = resX.attribute("name").value();
					std::string resPath = resX.attribute("location").value();
					zip_stat_t sbr;
					zip_stat_init(&sbr);
					zip_stat(zf, resPath.c_str(), 0, &sbr);
					if (!sbr.valid) throw std::runtime_error("Could not find resource with name (" + resName + "): " + resPath);

					auto indexFile = zip_fopen_index(zf, sbr.index, 0);
					std::unique_ptr<char[]> indexBuf(new char[sbr.size]);				
					zip_fread(indexFile, indexBuf.get(), sbr.size);
					
					resources.push_back({ resName, {indexBuf.get(), indexBuf.get() + sbr.size} });
				}
				block->name = name;
				block->logic()->loadResources(resources);
				globalBlocks.push_back(block);
			}
		}
	}
	zip_close(zf);
	_isOpen = true;
	return _isOpen;
}

bool Flow::FlowDocument::open(std::filesystem::path const& name) {
	setName(name);
	return open();
}

bool Flow::FlowDocument::isOpen() {
	return _isOpen;
}

void Flow::FlowDocument::setName(std::filesystem::path const& name) {
	filePath = name;
	pathSet = true;
}

bool Flow::FlowDocument::save() {
	if (!pathSet) throw std::runtime_error("Document has no path given to save to");
	//List of buffers to clear later to make LibZip happy
	std::vector<std::unique_ptr<char[]>> buffers; 
	int err = 0;
	//Open Zip Folder
	zip_error_t zerr;
	zip_error_init(&zerr);
	//Open zip
	zip_t* zf = zip_open(filePath.string().c_str(), ZIP_CREATE | ZIP_TRUNCATE, &err);
	if (err) {
		zip_error_init_with_code(&zerr, err);
		std::cerr << "Could not open file: " << filePath << std::endl
		          << "\tReason: " << zip_error_strerror(&zerr) << std::endl;
		return false;
	}
	//Prepare Index
	pugi::xml_document index;
	auto rootNode = index.append_child("FlowIndex");
	rootNode.append_attribute("version") = "0.0.1";

	//Process GlobalBlockData
	if (globalBlocks.size() > 0) {
		auto GlobalBlocks = rootNode.append_child("GlobalBlocks");
		for (auto& block : globalBlocks) {
			//Add block info to index xml tree
			auto blockX = GlobalBlocks.append_child("GlobalBlocks");
			blockX.append_attribute("name") = block->name.c_str();
			blockX.append_attribute("type") = block->getType().c_str();
			//Ask the block what files it wants to save
			auto resources = block->logic()->getResources();
			if (resources.size() > 0) {
				for (auto& res : resources) {
					std::string p = "blocks/" + block->name + "/" + res.name;
					//C people are weird
					auto data = res.data.data(); 
					auto size = res.data.size();
					//Need to save the data (from a vector<char>) or else res will go out of scope
					//And libzip doesn't actually read the data until zip_close
					buffers.push_back(std::make_unique<char[]>(size));
					std::copy(data, data + size, buffers.back().get());
					//Add to zip
					auto resSource = zip_source_buffer(zf, buffers.back().get(), size, 0);
					zip_file_add(zf, p.c_str(), resSource, ZIP_FL_OVERWRITE | ZIP_FL_ENC_UTF_8);
					//Add to xml index
					auto resEntry = blockX.append_child("Resource");
					resEntry.append_attribute("name") = res.name.c_str();
					resEntry.append_attribute("location") = p.c_str();
				}
			}
		}
	}

	//Add Index
	std::ostringstream docOut;
	index.save(docOut);
	auto docStr = docOut.str();
	auto indexSource = zip_source_buffer_create(docStr.c_str(), docStr.size(), 0, 0);
	zip_file_add(zf, "index.xml", indexSource, ZIP_FL_OVERWRITE | ZIP_FL_ENC_UTF_8);

	//Close file
	zip_close(zf);
	return true;
}

bool Flow::FlowDocument::save(std::filesystem::path const& name) {
	setName(name);
	return save();
}

bool Flow::FlowDocument::saveAs(std::filesystem::path const& name) {
	auto const oldPath = filePath;
	auto const oldPathSet = pathSet;
	setName(name);
	auto ret = save();
	filePath = oldPath;
	pathSet = oldPathSet;
	return ret;
}

void Flow::FlowDocument::close() {
	globalBlocks.clear();
	_isOpen = false;
}