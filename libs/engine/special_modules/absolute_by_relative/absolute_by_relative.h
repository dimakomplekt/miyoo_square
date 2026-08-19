// absolute_by_relative.h

#pragma once

// =========================================================================================== IMPORT

#include <string>

// =========================================================================================== IMPORT


/**
 * @brief Builds an absolute file path from a relative path based on the executable location.
 *
 * Combines the directory of the current executable with the given relative path to file from .exe folder,
 * producing a valid absolute path to a resource.
 *
 * @param relative Relative path to the target file (from the executable directory).
 *
 * @return Absolute path to the file (std::string).
 */
std::string absolute_by_relative_from_exe(const std::string& relative);
