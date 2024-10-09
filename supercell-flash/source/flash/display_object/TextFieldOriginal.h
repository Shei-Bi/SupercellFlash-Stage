#pragma once

#include "DisplayObjectOriginal.h"
#include "flash/types/SWFString.hpp"

namespace sc
{
	namespace flash {
		class SupercellSWF;
		class TextFieldOriginal : public DisplayObjectOriginal
		{
		public:
			TextFieldOriginal() {};
			virtual ~TextFieldOriginal() = default;

		public:
			enum class Align : uint8_t
			{
				Right,
				Center,
				Justify,
				Left,
			};

		public:
			SWFString text = "";

			SWFString font_name = "";
			uint32_t font_color = 0xFFFFFFFF;
			uint8_t font_size = 0;
			Align font_horizontal_align = Align::Left;
			Align font_vertical_align = Align::Left;
			bool unknown_align6 = false;
			bool unknown_align7 = false;

			int16_t left = 0;
			int16_t top = 0;
			int16_t right = 0;
			int16_t bottom = 0;

			bool is_bold = false;
			bool is_italic = false;
			bool is_multiline = false;
			bool is_outlined = false;
			bool unknown_flag3 = false;

			uint32_t outline_color = 0x000000FF;
			bool use_device_font = false;
			bool auto_kern = false;

			float bend_angle = 0.0f;

			bool unknown_flag = false;
			uint16_t unknown_short = 0xFFFF;
			uint16_t unknown_short2 = 0xFFFF;

			SWFString typography_file = "";

		public:
			virtual void load(SupercellSWF& swf, uint8_t tag);
			virtual void save(SupercellSWF& swf) const;
			virtual void save_data(SupercellSWF& swf, uint8_t tag) const;

			virtual uint8_t tag(SupercellSWF& swf) const;

			virtual bool is_textfield() const;
		};
	}
}
