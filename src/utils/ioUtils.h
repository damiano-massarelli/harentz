#ifndef IOUTILS_H
#define IOUTILS_H
#include <sstream>
#include <string>

/** \brief reads a file and returns it content
  * \param path the path of the file to read
  * \return a stringstream containing the content of the file.
  * If the file could not be opened the good() method of
  * the returned stringstream shall return false */
std::stringstream readFile(const std::string& path);

/** \brief writes a string into a file
  * \param path the path of the file
  * \param content the content of the file
  * \return true if successful, false otherwise */
bool writeFile(const std::string& path, const std::string& content);

#endif // IOUTILS_H
