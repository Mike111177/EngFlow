#pragma once
#include <Flow/blocks/AbstractCodeBlock.h>

namespace Flow {
	struct PythonBlockIMPL;
	class PythonBlock : public AbstractCodeBlock {
		std::unique_ptr<PythonBlockIMPL> impl;
	public:
		PythonBlock(std::weak_ptr<Block> p);
		virtual size_t nparams() override;
		virtual FlowVar execute(std::vector<FlowVar>) override;
		virtual void precompile() override;
		~PythonBlock();
	};
}