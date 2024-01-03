#include <peppermint-preview/Serialisable.h>

using namespace peppermint;

unsigned long long Serialisable::getDeserialisedSize() {
	return this->deserialisedSize;
}

void** Serialisable::getSerialisedID(void** in) {
	(*in) = this->serialisedID == nullptr ? this : this->serialisedID;
	return in;
}