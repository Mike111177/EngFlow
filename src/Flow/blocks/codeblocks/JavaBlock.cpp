#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <random>
#include <functional> 
#include <algorithm> 
#include <filesystem>
#include <fstream>

#include <jni.h>

#include <Util/TempFiles.h>
#include <Flow/blocks/codeblocks/JavaBlock.h>

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
		TempDir temp;
		auto sourceFileName = temp / "source.java";
		std::ofstream sourceFile(sourceFileName);
		sourceFile << source;
		sourceFile.close();
		//Compile File
		auto command = javacLoc + " " + sourceFileName.string();
		system(command.c_str());
		//Load Bytecode
		for (auto& p : temp) {
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
