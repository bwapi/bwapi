#pragma once

#include <filesystem>

namespace Util {

namespace FS = std::experimental::filesystem;

using Path = FS::path;
using FS::create_directories;

} // Util
