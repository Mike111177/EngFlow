#pragma once

#include <filesystem>

class TempDir: std::filesystem::directory_entry {
public:
	TempDir();
	~TempDir();
	std::filesystem::directory_iterator begin();
	std::filesystem::directory_iterator end();
	template <typename T>
	std::filesystem::path operator/(T const& t) {
		return path() / t;
	}

	

};

