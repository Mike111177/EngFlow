#include <string>

#include <Flow/FlowTypes.h>
#include <Flow/Module.h>

namespace Flow{
	class AbstractCodeBlock : public Block::Logic {
	protected:
		std::string source;
	public:
		using Logic::Logic;
		virtual void execute() = 0;
		virtual void precompile() = 0;
		void setSource(std::string);
	};
}