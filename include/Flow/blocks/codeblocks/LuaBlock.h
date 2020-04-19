#pragma once
#include <Flow/blocks/AbstractCodeBlock.h>

namespace Flow{
	struct LuaBlockIMPL;
	class LuaBlock : public AbstractCodeBlock {
		std::unique_ptr<LuaBlockIMPL> impl;
	protected:
		virtual FlowVar run(FlowVar args) override;
	public:
		static const std::string LogicType;
		LuaBlock(std::weak_ptr<Block> p);
		virtual size_t nparams() override;
		virtual bool precompile() override;
		~LuaBlock();
	};
}
