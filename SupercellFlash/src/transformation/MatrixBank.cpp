#include "SupercellFlash/transformation/MatrixBank.h"

namespace sc {
	MatrixBank::MatrixBank() {};
	MatrixBank::~MatrixBank() {
		for (const Matrix2x3* matrix : matrices) {
			if (matrix != NULL) {
				delete matrix;
			}
		}

		for (const ColorTransform* color : colorTransforms) {
			if (color != NULL) {
				delete color;
			}
		}

		matrices.clear();
		colorTransforms.clear();
	}

	bool MatrixBank::getMatrixIndex(Matrix2x3* matrix, uint16_t& index) {
		if (floatEqual(matrix->a, 1.0f) &&
			floatEqual(matrix->b, 0) &&
			floatEqual(matrix->c, 0) &&
			floatEqual(matrix->d, 1.0f) &&
			floatEqual(matrix->tx, 0) &&
			floatEqual(matrix->ty, 0)) {
			index = 0xFFFF;
			return true;
		}

		for (uint16_t i = 0; matrices.size() > i; i++) {
			Matrix2x3* m = matrices[i];

			if (*matrix == *m) {
				index = i;
				return true;
			}
		}

		return false;
	};

	bool MatrixBank::getColorTransformIndex(ColorTransform* color, uint16_t& index) {
		if (floatEqual(color->alpha, 1.0f) &&
			floatEqual(color->blueAdd, 0) &&
			floatEqual(color->blueMul, 1.0f) &&
			floatEqual(color->greenAdd, 0) &&
			floatEqual(color->greenMul, 1.0f) &&
			floatEqual(color->redAdd, 0) &&
			floatEqual(color->redMul, 1.0f)) {
			index = 0xFFFF;
			return true;
		}

		for (uint16_t i = 0; colorTransforms.size() > i; i++) {
			ColorTransform* c = colorTransforms[i];

			if (*color == *c) {
				index = i;
				return true;
			}
		}

		return false;
	}
}