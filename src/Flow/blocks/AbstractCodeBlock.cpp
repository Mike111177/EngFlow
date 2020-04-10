#include <Flow\blocks\AbstractCodeBlock.h>

void Flow::AbstractCodeBlock::setSource(std::string new_source) {
	source = new_source;
	precompile();
}
