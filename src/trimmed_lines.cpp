#include <istream>
#include <boost/algorithm/string/trim.hpp>
#include <blif/details/trimmed_lines.h>

bool blif::trimmed_lines::iterator::nextline() {
    do {
        line.clear();

        while (*pi) {
            std::string str;
            std::getline(*pi, str);

            // comment
            size_t pos = str.find('#');
            if (pos != std::string::npos) {
                str = str.substr(0, pos);
            }

            line += str;
            if (*line.rbegin() == '\\') {
                line.resize(line.size() - 1);
            }
            else {
                boost::algorithm::trim(line);
                break;
            }
        }

        if (!line.empty()) {
            return true;
        }
    } while (*pi);
    return false;
}
