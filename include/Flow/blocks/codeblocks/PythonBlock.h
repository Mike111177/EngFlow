#pragma once
#include <Flow/blocks/AbstractCodeBlock.h>

namespace Flow {
	struct PythonBlockIMPL;
	class PythonBlock : public AbstractCodeBlock {
		std::unique_ptr<PythonBlockIMPL> impl;
	protected:
		virtual FlowVar run(FlowVar args) override;
	public:
		static const std::string LogicType;
		PythonBlock(std::weak_ptr<Block> p);
		virtual std::vector<std::string> nparams() override;
		virtual bool precompile() override;
		~PythonBlock();
	};
}