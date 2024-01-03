#ifndef PPMINT_EDITOR_COMPONENT_PARAMETER_REGISTER_H
#define PPMINT_EDITOR_COMPONENT_PARAMETER_REGISTER_H

#include <vector>
#include <string>

namespace editor {
	namespace gui {
		class ComponentParameterRegister {
		public:
			enum ParameterTypes {
				INT,
				FLOAT,
				INTV2,
				FLOATV2,
				INTV3,
				FLOATV3,
				INTV4,
				FLOATV4,
				BOOL,
				DOUBLE,
				UINT,
				ENUM,
				VOID_P,
				VOID_PV,
				COMPONENT,
				ASSET,
				COMPONENTV,
				ASSETV,
				GAME_OBJECT,
				GAME_OBJECTV,
				STRING
			};

			struct ParameterData {
				/// <summary>
				/// A pointer to the actual parameter
				/// </summary>
				void* location = nullptr;
				/// <summary>
				/// The type of this parameter
				/// </summary>
				ParameterTypes type;
				/// <summary>
				/// The name to display for this parameter
				/// </summary>
				std::string name = "unnamed parameter";
				/// <summary>
				/// Whether or not to linearly map inputs to another set of outputs
				/// </summary>
				bool useMapping = false;
				/// <summary>
				/// Whether or not to clamp inputs.
				/// </summary>
				bool clamp = false;
				/// <summary>
				/// Minimum inputs and outputs
				/// </summary>
				float mins[2];
				/// <summary>
				/// Maximum inputs and outputs
				/// </summary>
				float maxes[2];
				/// <summary>
				/// Minimum inputs and outputs
				/// </summary>
				double doubleMins[2];
				/// <summary>
				/// Maximum inputs and outputs
				/// </summary>
				double doubleMaxes[2];
				/// <summary>
				/// Whether or not to show a separator in the inspector before this parameter
				/// </summary>
				bool separator = false;
				/// <summary>
				/// Text to display on a separator before this parameter
				/// </summary>
				std::string separatorText = "";
				std::string tooltip = "";

				/// <summary>
				/// For ASSET and COMPONENT, only allow objects with this display name
				/// </summary>
				std::string requiredTypeDisplayName = "";

				/// <summary>
				/// Values for stored enum names
				/// </summary>
				std::vector<unsigned int> enumValues;
				/// <summary>
				/// Display names for stored enum values
				/// </summary>
				std::vector<std::string> enumNames;

				bool disabled = false;

				bool requiresEnumValue = false;
				unsigned int requiredEnumValue = 0;

				ParameterData(std::string name, void* location, ParameterTypes type, bool useMapping = false, bool clamp = false) {
					this->name = name;
					this->location = location;
					this->type = type;
					this->useMapping = useMapping;
					this->clamp = clamp;
				}

				/// <summary>
				/// Set the range of inputs for this parameter.
				/// </summary>
				/// <param name="min">The minimum value.</param>
				/// <param name="max">The maximum value.</param>
				void setInRange(float min, float max) {
					this->mins[0] = min;
					this->maxes[0] = max;
				}

				/// <summary>
				/// Set the range of inputs for this parameter.
				/// </summary>
				/// <param name="min">The minimum value.</param>
				/// <param name="max">The maximum value.</param>
				void setInRange(double min, double max) {
					this->doubleMins[0] = min;
					this->doubleMaxes[0] = max;
				}

				/// <summary>
				/// Set the range of outputs for this parameter.
				/// </summary>
				/// <param name="min">The minimum value.</param>
				/// <param name="max">The maximum value.</param>
				void setOutRange(float min, float max) {
					this->mins[1] = min;
					this->maxes[1] = max;
				}

				/// <summary>
				/// Set the range of outputs for this parameter.
				/// </summary>
				/// <param name="min">The minimum value.</param>
				/// <param name="max">The maximum value.</param>
				void setOutRange(double min, double max) {
					this->doubleMins[1] = min;
					this->doubleMaxes[1] = max;
				}

				/// <summary>
				/// Map an input to an output.
				/// </summary>
				/// <param name="in">The value to map.</param>
				/// <returns></returns>
				float map(float in) {
					float slope = 1.0f * (maxes[1] - mins[1]) / (maxes[0] - mins[0]);
					return mins[1] + slope * (in - mins[0]);
				}

				/// <summary>
				/// Map an output to an input.
				/// </summary>
				/// <param name="in">The value to map.</param>
				/// <returns></returns>
				float unmap(float out) {
					float slope = 1.0f * (maxes[0] - mins[0]) / (maxes[1] - mins[1]);
					return mins[0] + slope * (out - mins[1]);
				}

				/// <summary>
				/// Map an input to an output.
				/// </summary>
				/// <param name="in">The value to map.</param>
				/// <returns></returns>
				double map(double in) {
					double slope = 1.0f * (doubleMaxes[1] - doubleMins[1]) / (doubleMaxes[0] - doubleMins[0]);
					return mins[1] + slope * (in - mins[0]);
				}

				/// <summary>
				/// Map an output to an input.
				/// </summary>
				/// <param name="in">The value to map.</param>
				/// <returns></returns>
				double unmap(double out) {
					double slope = 1.0f * (doubleMaxes[0] - doubleMins[0]) / (doubleMaxes[1] - doubleMins[1]);
					return mins[0] + slope * (out - mins[1]);
				}

				/// <summary>
				/// Activate/disable a separator before this parameter in the inspector.
				/// </summary>
				/// <param name="val">True/false depending on whether to display a separator.</param>
				void setPreSeparator(bool val) {
					this->separator = val;
				}

				/// <summary>
				/// Set the text that gets displayed on this parameter's separator.
				/// </summary>
				/// <param name="text">the text to display.</param>
				void setSeparatorText(std::string text) {
					this->separatorText = text;
				}

				/// <summary>
				/// Set the values of this enum parameter.
				/// </summary>
				/// <param name="vals"></param>
				void setEnumValues(std::vector<unsigned int> vals) {
					this->enumValues = vals;
				}

				/// <summary>
				/// Set the display names of the values of this enum parameter.
				/// </summary>
				/// <param name="names"></param>
				void setEnumNames(std::vector<std::string> names) {
					this->enumNames = names;
				}
			};

			std::vector<ParameterData> parameters;

			void clearParameters();
			std::vector<ParameterData>::iterator registerParameter(std::string name, void* location, ParameterTypes type, bool useMapping = false, bool clamp = false);
		};
	}
}

#endif