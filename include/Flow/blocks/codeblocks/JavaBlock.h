#pragma once
#include <Flow/blocks/AbstractCodeBlock.h>

namespace Flow {
	class JavaBlock : public AbstractCodeBlock {
		struct IMPL;
		std::unique_ptr<IMPL> impl;
	protected:
		virtual FlowVar run(FlowVar args) override;
	public:
		static void reg();
		static void setJavacLocation(std::string path);
		static const std::string LogicType;
		JavaBlock(std::weak_ptr<Block> p);
		virtual std::vector<std::string> nparams() override;
		virtual bool precompile() override;
		~JavaBlock();
	};
}
