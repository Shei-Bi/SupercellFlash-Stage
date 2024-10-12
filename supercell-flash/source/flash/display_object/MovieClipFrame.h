#pragma once

#include "flash/types/SWFString.hpp"


namespace sc
{
	namespace flash {
		class SupercellSWF;

		struct MovieClipFrameElement
		{
			uint16_t instance_index;
			uint16_t matrix_index = 0xFFFF;
			uint16_t colorTransform_index = 0xFFFF;
		};

		struct MovieClipFrame
		{
		public:
			MovieClipFrame() {};
			virtual ~MovieClipFrame() = default;

		public:
			uint16_t elements_count = 0;
			SWFString label;

		public:
			MovieClipFrameElement** frameElements;

		public:
			virtual void load(SupercellSWF& swf);
			virtual void save(SupercellSWF& swf) const;

			virtual uint8_t tag(SupercellSWF& swf) const;
		};
	}
}
