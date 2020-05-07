#include <string>
#include <algorithm>
#include <filesystem>
#include <exception>
#include <random>

#include <Util/TempFiles.h>

static std::string random_string(size_t length) {
	static const char vldchrs[] = {
		'0','1','2','3','4','5','6','7','8',
		'9','A','B','C','D','E','F','G','H',
		'I','J','K','L','M','N','O','P','Q',
		'R','S','T','U','V','W','X','Y','Z' };
	thread_local static std::default_random_engine rng(std::random_device{}());
	std::string str(length, 0);
	std::sample(vldchrs, vldchrs + sizeof(vldchrs), str.begin(), length, rng);
	return str;
}

std::filesystem::path tempdir() {
	auto length = 10;
	std::filesystem::path temppath;
	do {
		temppath = std::filesystem::temp_directory_path() / random_string(length);
	} while (std::filesystem::exists(temppath));
	std::filesystem::create_directory(temppath);
	return temppath;
}

TempDir::TempDir(): directory_entry(tempdir()) {}

TempDir::~TempDir() {
	if (exists()) {
		std::error_code err;
		std::filesystem::remove_all(path(), err);
	}
}

std::filesystem::directory_iterator TempDir::begin() {
	return std::filesystem::begin(std::filesystem::directory_iterator(*this));
}

std::filesystem::directory_iterator TempDir::end() {
	return std::filesystem::end(std::filesystem::directory_iterator(*this));
}
