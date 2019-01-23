#ifndef IOUTILS_H
#define IOUTILS_H
#include <sstream>
#include <string>

std::string pathWithPrefDir(const std::string& path, const std::string& org, const std::string& app);

/** \brief reads a file and returns it content
  * \param path the path of the file to read
  * \param usePrefDir if true the "pref dir" path is prepended to the provided path
  * \return a stringstream containing the content of the file.
  * If the file could not be opened the good() method of
  * the returned stringstream shall return false */
std::stringstream readFile(const std::string& path, bool usePrefDir = false);

/** \brief writes a string into a file
  * \param path the path of the file
  * \param content the content of the file
  * \param usePrefDir if true the "pref dir" path is prepended to the provided path
  * \return true if successful, false otherwise */
bool writeFile(const std::string& path, const std::string& content, bool usePrefDir = false);

#endif // IOUTILS_H
