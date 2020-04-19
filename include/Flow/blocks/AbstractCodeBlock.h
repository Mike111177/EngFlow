#pragma once
#include <string>

#include <Flow/FlowTypes.h>
#include <Flow/Block.h>
#include <Flow/Util.h>

namespace Flow{
	class AbstractCodeBlock : public Block::Logic {
		class CompileThread;
		static_ptr<AbstractCodeBlock, CompileThread> compiler;
	protected:
		std::string sourceExt = ".code"; //Derived classes may choose to change this
		virtual FlowVar run(FlowVar args) = 0;
		virtual size_t nparams() = 0;
	public:
		struct PData;
		std::unique_ptr<PData> pdata;
		std::string source;
		AbstractCodeBlock(std::weak_ptr<Block> b);
		virtual FlowResourceList& saveResources(FlowResourceList&) override;
		virtual void loadResources(FlowResourceList&) override;
		size_t params();
		void waitReady();
		FlowVar execute(FlowVar args);
		virtual bool precompile() = 0;
		void setSource(std::string);
		virtual ~AbstractCodeBlock();
	};
}