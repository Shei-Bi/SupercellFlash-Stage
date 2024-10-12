#pragma once

#include <any>
#include <optional>

#include <core/math/rect.h>

#include "flash/types/SWFString.hpp"
#include "flash/types/SWFContainer.hpp"

#include "flash/display_object/DisplayObjectOriginal.h"
#include "flash/display_object/MovieClipFrame.h"

namespace sc
{
	namespace flash {
		class SupercellSWF;

		struct MovieClipFrameElement;

		struct DisplayObjectInstance
		{
			enum class BlendMode : uint8_t
			{
				Normal = 0,
				// Normal1 = 1,
				Layer = 2,
				Multiply,
				Screen,
				Lighten,
				Darken,
				Difference,
				Add,
				Subtract,
				Invert,
				Alpha,
				Erase,
				Overlay,
				HardLight,
			};

			uint16_t id;
			BlendMode blend_mode = BlendMode::Normal;
			SWFString name;
		};

		class MovieClipOriginal : public DisplayObjectOriginal
		{
		public:
			MovieClipOriginal() {};
			virtual ~MovieClipOriginal() = default;

		public:
			SWFVector<MovieClipFrameElement, uint32_t> frame_elements;
			SWFVector<DisplayObjectInstance> instances;
			SWFVector<MovieClipFrame> frames;

		public:
			uint8_t frame_rate = 24;

			uint8_t bank_index = 0;

			std::optional<sc::RectF> scaling_grid;

			SWFVector<std::any, uint8_t> custom_properties;

		public:
			bool unknown_flag = false;

		public:
			virtual void load(SupercellSWF& swf, uint8_t tag);
			virtual void save(SupercellSWF& swf) const;

			virtual uint8_t tag(SupercellSWF& swf) const;

			virtual bool is_movieclip() const;
		public:
			DisplayObjectOriginal** displayObjects = nullptr;

			SWFString* name = nullptr;

			void createTimelineChildren(SupercellSWF* swf);
		};
	}
}
