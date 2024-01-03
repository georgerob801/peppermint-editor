#include <peppermint-preview/classes/game/components/renderers/SpriteRenderer.h>

using namespace peppermint::game::components;

SpriteRenderer::SpriteRenderer() {
	this->type = Component::SPRITE_RENDERER;
	this->textureSet = nullptr;

	this->displayTypeName = "sprite renderer";

	this->registerParameter("texture set", &this->textureSet, editor::gui::ComponentParameterRegister::ASSET);
	auto settings = this->registerParameter("position", &this->position, FLOATV2);
	(*settings).tooltip = "the pixel position of the centre of the area to display";
	(*settings).clamp = true;
	(*settings).setInRange(FLT_MIN, FLT_MAX);
	settings = this->registerParameter("size", &this->size, FLOATV2);
	(*settings).tooltip = "the size in pixels of the area to display";
	(*settings).clamp = true;
	(*settings).setInRange(FLT_MIN, FLT_MAX);

	this->description = "renders a static sprite";
	this->iconIndex = 10;

	this->bufferType = 0x88e8;
}

void SpriteRenderer::editorLoop() {
	if (this->lastPosition.x != this->position.x || this->lastPosition.y != this->position.y || this->lastSize.x != this->size.x || this->lastSize.y != this->size.y) {
		this->generateVertices();
		this->requiresRemeshing = true;
		this->lastPosition = this->position;
		this->lastSize = this->size;
	}
}

void SpriteRenderer::generateTextures() {
	this->textureSet->generateAtlas();
}

void SpriteRenderer::generateVertices() {
	this->vertices.clear();
	vector<Vertex>().swap(this->vertices);

	// once again should be testing only
	this->textures.push_back(this->textureSet->atlas);

	vector<Vertex> square = Mesh::SQUARE();

	float ratio = this->size.x / this->size.y;

	for (unsigned int i = 0; i < 4; i++) {
		vec3* posP = &square[i].position;

		if (ratio < 1.0f) posP->y *= ratio;
		else posP->x *= ratio;

		vec2* uvP = &square[i].uv;
		// make uv go in right place
		*uvP -= vec2(0.5f, 0.5f);
		*uvP *= 2;
		if (uvP->x == -1) {
			uvP->x = (this->position.x / this->textureSet->atlas->getWidth()) - ((this->size.x / this->textureSet->atlas->getWidth()) / 2.0f);
		} else {
			uvP->x = (this->position.x / this->textureSet->atlas->getWidth()) + ((this->size.x / this->textureSet->atlas->getWidth()) / 2.0f);
		}
		if (uvP->y == -1) {
			uvP->y = (this->position.y / this->textureSet->atlas->getHeight()) - ((this->size.y / this->textureSet->atlas->getHeight()) / 2.0f);
		} else {
			uvP->y = (this->position.y / this->textureSet->atlas->getHeight()) + ((this->size.y / this->textureSet->atlas->getHeight()) / 2.0f);
		}

		this->vertices.push_back(square[i]);
	}
}

vector<byte> SpriteRenderer::serialise() {
	vector<byte> out;

	unsigned int e = this->type;
	byte* toAdd = reinterpret_cast<byte*>(&e);
	for (unsigned int i = 0; i < sizeof(unsigned int); i++) {
		out.push_back(toAdd[i]);
	}

	void* id = this->serialisedID == nullptr ? this : this->serialisedID;
	byte* toAdd2 = reinterpret_cast<byte*>(&id);
	for (unsigned int i = 0; i < sizeof(void*); i++) {
		out.push_back(toAdd2[i]);
	}

	void* sID = nullptr;
	byte* textureSetIDB = reinterpret_cast<byte*>(this->textureSet->getSerialisedID(&sID));
	for (unsigned int i = 0; i < sizeof(void*); i++) {
		out.push_back(textureSetIDB[i]);
	}

	vector<float> floatsToAdd;
	floatsToAdd.push_back(this->position.x);
	floatsToAdd.push_back(this->position.y);
	floatsToAdd.push_back(this->size.x);
	floatsToAdd.push_back(this->size.y);

	for (unsigned int i = 0; i < floatsToAdd.size(); i++) {
		byte* floatB = reinterpret_cast<byte*>(&floatsToAdd[i]);
		for (unsigned int j = 0; j < sizeof(float); j++) {
			out.push_back(floatB[j]);
		}
	}

	return out;
}

void SpriteRenderer::deserialise(vector<byte> bytes) {
	unsigned long long position = 0x00;

	this->serialisedID = *reinterpret_cast<void**>(&bytes[position]);
	position += sizeof(void*);

	this->relatedSerialisedIDs.push_back(*reinterpret_cast<void**>(&bytes[position]));
	position += sizeof(void*);

	this->position.x = *reinterpret_cast<float*>(&bytes[position]);
	position += sizeof(float);
	this->position.y = *reinterpret_cast<float*>(&bytes[position]);
	position += sizeof(float);
	this->size.x = *reinterpret_cast<float*>(&bytes[position]);
	position += sizeof(float);
	this->size.y = *reinterpret_cast<float*>(&bytes[position]);
	position += sizeof(float);

	this->deserialisedSize = position;
}