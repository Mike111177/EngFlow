#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <random>
#include <functional> 
#include <algorithm> 
#include <filesystem>
#include <fstream>

#include <jni.h>

#include <Flow/blocks/codeblocks/JavaBlock.h>

//Temp file util, should move to a util header at some point
typedef std::vector<char> char_array;
char_array charset() {
	//Change this to suit
	return char_array(
		{ '0','1','2','3','4',
		'5','6','7','8','9',
		'A','B','C','D','E','F',
		'G','H','I','J','K',
		'L','M','N','O','P',
		'Q','R','S','T','U',
		'V','W','X','Y','Z',
		'a','b','c','d','e','f',
		'g','h','i','j','k',
		'l','m','n','o','p',
		'q','r','s','t','u',
		'v','w','x','y','z'
		});
};
std::string random_string(size_t length, std::function<char(void)> rand_char) {
	std::string str(length, 0);
	std::generate_n(str.begin(), length, rand_char);
	return str;
}
std::filesystem::path tempfile() {
	const auto ch_set = charset();
	std::default_random_engine rng(std::random_device{}());
	std::uniform_int_distribution<> dist(0, ch_set.size() - 1);
	auto randchar = [ch_set, &dist, &rng]() {return ch_set[dist(rng)]; };
	auto length = 10;
	return /*std::filesystem::temp_directory_path() /*/ random_string(length, randchar);
}


const std::string Flow::JavaBlock::LogicType = "JavaBlock";

static std::string javacLoc;
static bool javacLocSet = false;

struct Flow::JavaBlock::IMPL {
	std::vector<std::string> nparams() {
		return {};
	}
	FlowVar run(FlowVar args) { 
		return {};
	}
	bool precompile(std::string & source) { 
		if (!javacLocSet) throw std::runtime_error("JavacLoc not set!");
		//Save File
		auto temp = tempfile();
		std::filesystem::create_directory(temp);
		auto sourceFileName = temp / "source.java";
		std::ofstream sourceFile(sourceFileName);
		sourceFile << source;
		sourceFile.close();
		//Compile File
		auto command = javacLoc + " " + sourceFileName.string();
		system(command.c_str());
		//Load Bytecode
		for (auto& p : std::filesystem::directory_iterator(temp)) {
			if (p.is_regular_file() && p.path().extension() == ".class") {
				std::cout << p.path() << std::endl;
			}
		}
		//Reflect
		return false; 
	}
};

//Pimpl forward
Flow::JavaBlock::JavaBlock(std::weak_ptr<Block> p) : AbstractCodeBlock(p), impl(new IMPL) { sourceExt = ".java"; }
std::vector<std::string> Flow::JavaBlock::nparams() { return impl->nparams(); }
Flow::FlowVar Flow::JavaBlock::run(FlowVar args) { return impl->run(args); }
bool Flow::JavaBlock::precompile() { return impl->precompile(source); }
Flow::JavaBlock::~JavaBlock() {}

//Static Register Function 
void Flow::JavaBlock::reg() { Block::registerLogic<JavaBlock>(); }


void Flow::JavaBlock::setJavacLocation(std::string path) {
	//This has to go eventually
	std::cout << "TODO: This is stupid, use a config file or something (" << __FILE__ << ": " << __LINE__ << ")" << std::endl;
	javacLoc = path;
	javacLocSet = true;
}
