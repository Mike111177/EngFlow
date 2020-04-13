#include <stdexcept>
#include <sstream>
#include <random>

#include <pugixml.hpp>
#include <minizip/zip.h>
#include <minizip/unzip.h>
#include <minizip/mz_strm_zlib.h>

#include <Flow/FlowDocument.h>

static const char alphanum[] =
"0123456789"
"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
"abcdefghijklmnopqrstuvwxyz";
static std::default_random_engine rgen;
static std::uniform_int_distribution<size_t> alnumd(0, sizeof(alphanum) - 1);

inline std::string randStr(size_t const &len) {
	std::string ret;
	ret.resize(len);
	for (auto& c : ret) c = alphanum[alnumd(rgen)];
	return ret;
}

std::filesystem::path getNewTempFolder() {
	using namespace std::filesystem;
	path tpath;
	do { tpath = temp_directory_path()/randStr(10); } while (exists(tpath));
	return tpath;
}

bool Flow::FlowDocument::spawnTempDir() {
	destroyTempDir();
	return tempDirSpawned = std::filesystem::create_directory(tempFolder = getNewTempFolder());
}

bool Flow::FlowDocument::destroyTempDir() {
	if (tempDirSpawned && std::filesystem::exists(tempFolder)) {
		std::filesystem::remove_all(tempFolder);
	}
	tempDirSpawned = false;
	return true;
}

Flow::FlowDocument::FlowDocument() {}

Flow::FlowDocument::FlowDocument(std::filesystem::path const &name): FlowDocument() {setName(name);}

Flow::FlowDocument::~FlowDocument() { close(); }

bool Flow::FlowDocument::create() {
	index.reset();
	auto rootNode = index.append_child("FlowIndex");
	rootNode.append_attribute("version") = "0.0.1";
	auto modules = rootNode.append_child("Modules");
	auto mainModule = modules.append_child("Module");
	mainModule.append_attribute("id") = "main";
	rootNode.append_child("Resources");

	_isOpen = true;
	return false;
}

bool Flow::FlowDocument::open() {
	if (!pathSet) throw std::runtime_error("Document has no path given to load from");
	//Load zip file zip
	//Read index.xml
	//node m = "Modules"
	//for every c in m
		//folder = c.attr("name")
	    //auto currMod = new Module();
	    //modules.push_back(currMod);
	    //Read modules/folder/moduledata.xml
		//node b = "Blocks"
		//for cb in b
			//name = cb.attr("name")
			//blocktype = cb.attr("type")
	        //DataReader dr(zip, Resources/blocks/module_folder/name)
	        //auto block = Flow::Block::create(blocktype);
	        //block->load(dr);
	        //currMod->addBlock(block)
	spawnTempDir();
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
	//Prepare Index File
	std::ostringstream docOut;
	index.save(docOut);
	std::string doc = docOut.str();

	//Open Zip Folder
	zip_fileinfo zfi = { 0 };
	zipFile zf = zipOpen(filePath.string().c_str(), APPEND_STATUS_CREATE);

	//Add Index
	zipOpenNewFileInZip(zf, "index.xml", &zfi, NULL, 0, NULL, 0, NULL, 8, -1);
	zipWriteInFileInZip(zf, doc.data(), doc.size());
	zipCloseFileInZip(zf);

	//Close Zip
	zipClose(zf, NULL);
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
	if (_isOpen) {
		destroyTempDir();
	}
	_isOpen = false;
}