#include <peppermint-preview/classes/Asset.h>

#include <format>

using namespace peppermint;

void Asset::setupDisplayTypeEnum() {
	auto settings = this->registerParameter("type", &this->type, ENUM);

	(*settings).disabled = true;

	vector<unsigned int> enumVals;
	enumVals.push_back(AssetType::NONE);
	enumVals.push_back(AssetType::IMAGE);
	enumVals.push_back(AssetType::TILESET_ANIMATION);
	enumVals.push_back(AssetType::FRAGMENT_SHADER_SOURCE);
	enumVals.push_back(AssetType::VERTEX_SHADER_SOURCE);
	enumVals.push_back(AssetType::TEXTURE_SET);
	enumVals.push_back(AssetType::TEXTURE);
	enumVals.push_back(AssetType::TILESET);
	enumVals.push_back(AssetType::PPMINT_WORLD_FILE);
	enumVals.push_back(AssetType::PPMINT_GAME_FILE);
	enumVals.push_back(AssetType::PPMINT_ASSET_FILE);
	enumVals.push_back(AssetType::SOUND);
	enumVals.push_back(AssetType::FONT);
	enumVals.push_back(AssetType::SHADER);

	(*settings).enumValues = enumVals;

	vector<std::string> enumNames;
	enumNames.push_back("none");
	enumNames.push_back("image");
	enumNames.push_back("tileset animation");
	enumNames.push_back("fragment shader source");
	enumNames.push_back("vertex shader source");
	enumNames.push_back("texture set");
	enumNames.push_back("texture");
	enumNames.push_back("tileset");
	enumNames.push_back("world file");
	enumNames.push_back("game file");
	enumNames.push_back("asset file");
	enumNames.push_back("sound");
	enumNames.push_back("font");
	enumNames.push_back("shader");

	(*settings).enumNames = enumNames;
}

Asset::Asset(AssetType type) {
	this->type = type;

	this->displayTypeName = "asset";
	this->displayType = editor::gui::GUIInfo::ASSET;
	this->iconIndex = 11;

	this->setupDisplayTypeEnum();

	this->registerParameter("path", &this->path, STRING);
}

Asset::Asset(AssetType type, char* path) {
	this->type = type;
	this->path = path;

	this->displayTypeName = "asset";
	this->displayType = editor::gui::GUIInfo::ASSET;

	this->setupDisplayTypeEnum();
}

void Asset::setPath(const char* path) {
	this->setPath((char*)path);
}

void Asset::setPath(char* path) {
	this->path = path;
}

vector<byte> Asset::serialise() {
	vector<byte> out;

	unsigned int typeCast = (unsigned int)this->type;
	byte* typeB = reinterpret_cast<byte*>(&typeCast);
	for (unsigned int i = 0; i < sizeof(unsigned int); i++) {
		out.push_back(typeB[i]);
	}

	void* id = this->serialisedID == nullptr ? this : this->serialisedID;
	byte* idB = reinterpret_cast<byte*>(&id);
	for (unsigned int i = 0; i < sizeof(void*); i++) {
		out.push_back(idB[i]);
	}

	if (this->path.length() != 0) {
		std::string pathStr(this->path);

		unsigned int length = pathStr.size();
		byte* pathSizeB = reinterpret_cast<byte*>(&length);
		for (unsigned int i = 0; i < sizeof(unsigned int); i++) {
			out.push_back(pathSizeB[i]);
		}

		const char* e = this->path.c_str();
		byte* pathB = reinterpret_cast<byte*>(&e);
		for (unsigned int i = 0; i < length * sizeof(char); i++) {
			out.push_back(pathB[i]);
		}
	} else {
		unsigned int length = 0;
		byte* pathSizeB = reinterpret_cast<byte*>(&length);
		for (unsigned int i = 0; i < sizeof(unsigned int); i++) {
			out.push_back(pathSizeB[i]);
		}
	}

	//out += "Asset:\n";
	//out += std::format("ID: {}\n", this->id);
	//out += "Data:\n";
	//out += std::format("Type: {}\n", (unsigned int)this->type);
	//out += std::format("Path: {}\n", this->path);

	return out;
}

#include <iostream>

void Asset::deserialise(vector<byte> bytes) {
	unsigned long long position = 0x00;

	this->serialisedID = *reinterpret_cast<void**>(&bytes[position]);
	position += sizeof(void*);

	unsigned int pathLength = *reinterpret_cast<unsigned int*>(&bytes[position]);
	position += sizeof(unsigned int);

	std::string ePath = "";

	for (unsigned int i = 0; i < pathLength; i++) {
		ePath += *reinterpret_cast<char*>(&bytes[position]);
		position += sizeof(char);
	}

	this->path = ePath;

	this->deserialisedSize = sizeof(void*) + sizeof(unsigned int) + (ePath.size() * sizeof(char));
}