#pragma once

#include <filesystem>

namespace Util {

namespace FS = std::filesystem;

using Path = FS::path;
using FS::create_directories;

} // Util
