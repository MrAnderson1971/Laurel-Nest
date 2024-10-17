#include <memory>

inline std::unique_ptr<Region> makeCesspit() {
	return std::make_unique<Cesspit>();
}
