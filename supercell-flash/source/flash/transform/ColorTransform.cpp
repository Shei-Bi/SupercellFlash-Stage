#include "ColorTransform.h"

#include "flash/objects/SupercellSWF.h"

namespace sc
{
	namespace flash {
		void ColorTransform::load(SupercellSWF& swf)
		{
			add.r = swf.stream.read_unsigned_byte();
			add.g = swf.stream.read_unsigned_byte();
			add.b = swf.stream.read_unsigned_byte();

			alpha = swf.stream.read_unsigned_byte();

			multiply.r = swf.stream.read_unsigned_byte();
			multiply.g = swf.stream.read_unsigned_byte();
			multiply.b = swf.stream.read_unsigned_byte();
		}

		void ColorTransform::save(SupercellSWF& swf) const
		{
			swf.stream.write_unsigned_byte(add.r);
			swf.stream.write_unsigned_byte(add.g);
			swf.stream.write_unsigned_byte(add.b);

			swf.stream.write_unsigned_byte(alpha);

			swf.stream.write_unsigned_byte(multiply.r);
			swf.stream.write_unsigned_byte(multiply.g);
			swf.stream.write_unsigned_byte(multiply.b);
		}

		bool ColorTransform::operator==(const ColorTransform& color) const
		{
			if (color.alpha == alpha &&
				color.add.r == add.r &&
				color.add.g == add.g &&
				color.add.b == add.b &&
				color.multiply.r == multiply.r &&
				color.multiply.g == multiply.g &&
				color.multiply.b == multiply.b) {
				return true;
			}

			return false;
		}

		void ColorTransform::multiplyy(ColorTransform* color) {
			multiply.r = fmin(multiply.r * color->multiply.r / 255.0f, 255.0f);
			multiply.g = fmin(multiply.g * color->multiply.g / 255.0f, 255.0f);
			multiply.b = fmin(multiply.b * color->multiply.b / 255.0f, 255.0f);
			alpha = fmin(alpha * color->alpha / 255.0f, 255.0f);
			add.r = fmin(add.r + color->add.r, 255.0f);
			add.g = fmin(add.g + color->add.g, 255.0f);
			add.b = fmin(add.b + color->add.b, 255.0f);
		}

		uint8_t ColorTransform::tag(SupercellSWF&) const
		{
			return TAG_COLOR_TRANSFORM;
		}
	}
}