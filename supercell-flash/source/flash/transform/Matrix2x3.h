#pragma once

#include <core/math/matrix2d.h>

namespace sc
{
	namespace flash {
		class SupercellSWF;

		struct Matrix2x3 : public sc::Matrix2D
		{
		public:
			Matrix2x3() {};
			virtual ~Matrix2x3() = default;

		public:
			virtual void load(SupercellSWF& swf, uint8_t tag);
			virtual void save(SupercellSWF& swf) const;

			virtual uint8_t tag(SupercellSWF& swf) const;

		public:
			bool operator==(const Matrix2x3& matrix) const;

		public:
			void multiply(Matrix2x3* matrix);
			float applyX(float, float);
			float applyY(float, float);
		};
	}
}
