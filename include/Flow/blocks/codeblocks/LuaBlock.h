#pragma once
#include <Flow/blocks/AbstractCodeBlock.h>

namespace Flow{
	struct LuaBlockIMPL;
	class LuaBlock : public AbstractCodeBlock {
		std::unique_ptr<LuaBlockIMPL> impl;
	public:
		LuaBlock(std::weak_ptr<Block> p);
		virtual size_t nparams() override;
		virtual FlowVar execute(std::vector<FlowVar>) override;
		virtual void precompile() override;
		~LuaBlock();
	};
}
