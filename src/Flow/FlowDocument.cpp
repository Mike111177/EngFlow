#include <stdexcept>
#include <sstream>
#include <random>

#include <pugixml.hpp>
#include <minizip/zip.h>
#include <minizip/mz_strm_zlib.h>

#include <Flow/FlowDocument.h>

#ifdef _MSC_VER
#pragma comment(lib, "minizip.lib")
#pragma comment(lib, "zlibstaticd.lib")
#pragma comment(lib, "pugixml.lib")
#endif


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

Flow::FlowDocument::FlowDocument(): tempFolder(getNewTempFolder()) {
	auto s = std::filesystem::create_directory(tempFolder);
#ifdef _DEBUG
	std::cout << (s ? "Successfully created temporary folder: " 
		            : "Failed to create temporary folder: ")
		      << tempFolder << std::endl;
#endif
}

Flow::FlowDocument::FlowDocument(std::filesystem::path const &name): FlowDocument() {setName(name);}

Flow::FlowDocument::~FlowDocument() {}

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
	std::filesystem::remove_all(tempFolder);
	_isOpen = false;
}