#include "Matrix2x3.h"

#include "flash/objects/SupercellSWF.h"

#define floatEqual(a,b) (fabs(a - b) <= ( (fabs(a) < fabs(b) ? fabs(b) : fabs(a)) * 0.001))

namespace sc {
	namespace flash {
		void Matrix2x3::load(SupercellSWF& swf, uint8_t tag)
		{
			float divider = tag == TAG_MATRIX_2x3 ? 1024.0f : 65535.0f;

			a = float(swf.stream.read_int() / divider);
			b = float(swf.stream.read_int() / divider);
			c = float(swf.stream.read_int() / divider);
			d = float(swf.stream.read_int() / divider);

			tx = swf.stream.read_twip();
			ty = swf.stream.read_twip();
		}

		void Matrix2x3::save(SupercellSWF& swf) const
		{
			float multiplier = swf.use_precision_matrix ? 65535.0f : 1024.0f;

			swf.stream.write_int((int)(a * multiplier));
			swf.stream.write_int((int)(b * multiplier));
			swf.stream.write_int((int)(c * multiplier));
			swf.stream.write_int((int)(d * multiplier));

			swf.stream.write_twip(tx);
			swf.stream.write_twip(ty);
		}

		bool Matrix2x3::operator==(const Matrix2x3& matrix) const
		{
			if (floatEqual(a, matrix.a) &&
				floatEqual(b, matrix.b) &&
				floatEqual(c, matrix.c) &&
				floatEqual(d, matrix.d) &&
				floatEqual(tx, matrix.tx) &&
				floatEqual(ty, matrix.ty)) {
				return true;
			}

			return false;
		}

		float Matrix2x3::applyX(float x, float y) {
			return x * a + y * c + tx;
		}

		float Matrix2x3::applyY(float x, float y) {
			return y * d + x * b + ty;
		}
		void Matrix2x3::multiply(Matrix2x3* matrix) {
			float scaleX = (a * matrix->a) + (b * matrix->c);
			float shearX = (a * matrix->b) + (b * matrix->d);
			float scaleY = (d * matrix->d) + (c * matrix->b);
			float shearY = (d * matrix->c) + (c * matrix->a);
			float x = matrix->applyX(tx, ty);
			float y = matrix->applyY(tx, ty);

			a = scaleX;
			b = shearX;
			d = scaleY;
			c = shearY;
			tx = x;
			ty = y;
		}

		uint8_t Matrix2x3::tag(SupercellSWF& swf) const
		{
			return swf.use_precision_matrix ? TAG_MATRIX_2x3_2 : TAG_MATRIX_2x3;
		};
	}
}