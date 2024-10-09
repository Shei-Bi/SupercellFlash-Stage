#pragma once

#include "DisplayObjectOriginal.h"
#include "ShapeDrawBitmapCommand.h"

#include "flash/types/SWFContainer.hpp"

namespace sc
{
	namespace flash {
		class SupercellSWF;
		class ShapeOriginal : public DisplayObjectOriginal
		{
		public:
			ShapeOriginal() {};
			virtual ~ShapeOriginal() = default;

		public:
			SWFVector<ShapeDrawBitmapCommand> commands;

		public:
			void load(SupercellSWF& swf, uint8_t tag);
			void save(SupercellSWF& swf) const;

			virtual uint8_t tag(SupercellSWF& swf) const;

			virtual bool is_shape() const;
		};
	}
}
