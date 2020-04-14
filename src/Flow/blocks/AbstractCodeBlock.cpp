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

Flow::FlowResourceList Flow::AbstractCodeBlock::getResources() {
	return { {"source" + sourceExt, {source.begin(), source.end()}} };
}

void Flow::AbstractCodeBlock::loadResources(FlowResourceList& resources) {
	for (auto& res : resources) {
		if (res.name == "source" + sourceExt) {
			setSource({ res.data.data(), res.data.data() + res.data.size() });
			break;
		}
	}
}

void Flow::AbstractCodeBlock::setSource(std::string new_source) {
	source = new_source;
	precompile();
}
