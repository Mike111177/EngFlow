#include <iostream>
#include <stdexcept>
#include <algorithm> 
#include <fstream>

#include <jni.h>

#include <Util/TempFiles.h>
#include <Util/SecureSystem.h>
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
		//Create Temp Dir
		TempDir temp;

		//Create Source File
		//Ex... %Temp%/4j783jy7f3jy879/source.java
		auto sourceFileName = temp / "source.java"; 
		std::ofstream sourceFile(sourceFileName);
		sourceFile << source;
		sourceFile.close();

		//Compile File
		//This is not throwing, so this thinks its there...
		if (!std::filesystem::exists(sourceFileName)) throw "I can't see the file"; 
		
        //If I smack a breakpoint here and go to the temp folder, source.java definitley exist
		//If I then wait like 5-10 seconds before resuming it works sometimes......

		// Essentially runs "javac.exe source.java"
		// Will probably fail due to not finding the file
		secsystem(javacLoc, { sourceFileName.string() }); 

		//Load Bytecode
		for (auto& p : temp) {
			if (p.is_regular_file() && p.path().extension() == ".class") {
				std::cout << p.path() << std::endl;
			}
		}
		//Reflect
		return false; //Not fully implemented, don't allow execution
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
