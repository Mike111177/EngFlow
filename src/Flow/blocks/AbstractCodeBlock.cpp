#include <Flow\blocks\AbstractCodeBlock.h>

bool Flow::AbstractCodeBlock::loadBinary() {
	return false;
}

void Flow::AbstractCodeBlock::saveBinary() {}

void Flow::AbstractCodeBlock::load() {
	//source = dr.read("Source" + sourceExt);
	if (!loadBinary()) {
		precompile();
	}
}

void Flow::AbstractCodeBlock::save() {
	//dr.write("Source" + sourceExt, source);
	saveBinary(/*dr*/);
}

void Flow::AbstractCodeBlock::setSource(std::string new_source) {
	source = new_source;
	precompile();
}
