#pragma once
#include <string>

#include <Flow/FlowTypes.h>
#include <Flow/Block.h>

namespace Flow{
	class AbstractCodeBlock : public Block::Logic {
	protected:
		std::string sourceExt = ".code"; //Derived classes may choose to change this
		std::string source;
	public:
		using Logic::Logic;
		virtual FlowResourceList& saveResources(FlowResourceList&) override;
		virtual void loadResources(FlowResourceList&) override;
		virtual size_t nparams() = 0;
		virtual FlowVar execute(FlowVar args) = 0;
		virtual void precompile() = 0;
		void setSource(std::string);
	};
}