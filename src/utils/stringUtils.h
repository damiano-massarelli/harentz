#ifndef STRINGUTILS_H
#define STRINGUTILS_H

template<typename T>
std::vector<T> split(const std::string &toSplit, char ch, std::function<T(std::string& str)> converter)
{
    std::stringstream ss(toSplit);
    std::vector<T> result;

    while(ss.good()) {
        std::string substr;
        getline(ss, substr, ch);
        result.push_back(converter(substr));
    }

    return result;
}

#endif // STRINGUTILS_H
