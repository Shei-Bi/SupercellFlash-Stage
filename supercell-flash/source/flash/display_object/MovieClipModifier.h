#pragma once

#include "DisplayObjectOriginal.h"
#include "flash/flash_tags.h"

namespace sc
{
	namespace flash {
		class SupercellSWF;

		class MovieClipModifier : public DisplayObjectOriginal
		{
		public:
			MovieClipModifier() {};
			virtual ~MovieClipModifier() = default;

		public:
			enum class Type : uint8_t
			{
				Mask = TAG_MOVIE_CLIP_MODIFIER,
				Masked = TAG_MOVIE_CLIP_MODIFIER_2,
				Unmasked = TAG_MOVIE_CLIP_MODIFIER_3,
			};

		public:
			Type type = Type::Mask;

		public:
			virtual void load(SupercellSWF& swf, uint8_t tag);
			virtual void save(SupercellSWF& swf) const;

			virtual uint8_t tag(SupercellSWF& swf) const;

			virtual bool is_modifier() const;
		};
	}
}
