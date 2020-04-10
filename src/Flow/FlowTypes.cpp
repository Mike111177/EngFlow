#include <Flow/FlowTypes.h>

bool Flow::FlowTypeConstraint::test(FlowVar* fv) {
	return test(fv->getType());
}
