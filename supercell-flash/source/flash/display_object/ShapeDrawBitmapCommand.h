#pragma once

#include "flash/types/SWFContainer.hpp"

class GLImage;

namespace sc
{
	namespace flash {
		class SupercellSWF;

		struct ShapeDrawBitmapCommandVertex
		{
			float x;
			float y;

			float u;
			float v;
		};

		class ShapeDrawBitmapCommand
		{
		public:
			ShapeDrawBitmapCommand() {};
			virtual ~ShapeDrawBitmapCommand() = default;

		public:
			uint8_t texture_index = 0;
			SWFVector<ShapeDrawBitmapCommandVertex> vertices;

		public:
			virtual void load(SupercellSWF& swf, uint8_t tag);
			virtual void save(SupercellSWF& swf) const;

			virtual uint8_t tag(SupercellSWF& swf) const;

		public:
			GLImage* GLImage;
		};
	}
}
