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

#endif // IOUTILS_H
