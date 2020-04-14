#include <Flow\Block.h>
#include <Flow\CorePlugin.h>

#include <Flow/blocks/codeblocks/LuaBlock.h>
#include <Flow/blocks/codeblocks/PythonBlock.h>

template std::shared_ptr<Flow::Block> Flow::Block::create<Flow::PythonBlock>();
template std::shared_ptr<Flow::Block> Flow::Block::create<Flow::LuaBlock>();

static bool inited = false;

void initCoreComponents(){
	if (!inited) {
		Flow::Block::Registry.insert({ "PythonBlock", Flow::Block::create<Flow::PythonBlock> });
		Flow::Block::Registry.insert({ "LuaBlock", Flow::Block::create<Flow::LuaBlock> });
	}
	inited = true;
}