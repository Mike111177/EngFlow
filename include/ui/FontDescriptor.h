#pragma once
#include <string>
struct FontDescriptor {
	std::string fontname;
	int rSize;
};
inline bool operator==(FontDescriptor const& a, FontDescriptor const& b) {
    return a.fontname == b.fontname && a.rSize == b.rSize;
}
template<> struct std::hash<FontDescriptor> {
    std::size_t operator()(FontDescriptor const& s) const noexcept {
        std::size_t h1 = std::hash<std::string>{}(s.fontname);
        std::size_t h2 = std::hash<int>{}(s.rSize);
        return h1 ^ (h2 << 1);
    }
};