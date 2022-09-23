#include <random>

namespace rnd {
	float generateRandomFloat(float a, float b) {
		std::random_device dev;
		std::mt19937 rng(dev());
		std::uniform_real_distribution <float> distribution(a, b);

		return distribution(rng);
	}
}