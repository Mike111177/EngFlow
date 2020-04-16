#include <Flow\blocks\AbstractCodeBlock.h>

Flow::FlowResourceList& Flow::AbstractCodeBlock::saveResources(FlowResourceList &resList) {
	resList.emplace_back("source" + sourceExt, std::vector<char>{ source.begin(), source.end() });
	return resList;
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
