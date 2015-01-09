#include <cassert>
#include <string>
#include <vector>
#include <boost/algorithm/string.hpp>
#include <boloq/boolean_function.h>
#include <blif/details/trimmed_lines.h>
#include <blif/gate.h>

namespace blif {

static bool continue_line(const std::string& line) {
    return !line.empty() && (line[0] == '0' || line[0] == '1' || line[0] == '-');
}

static boloq::boolean_function parse_term_line(const std::string& line) {
    using namespace boloq;
    boolean_function term = boolean_function::one();
    for (size_t i = 0; i < line.size() && line[i] != ' '; ++i) {
        if (line[i] == '-') continue;
        boolean_function v(i);
        if (line[i] == '1') {
            term &= v;
        }
        else if (line[i] == '0') {
            term &= ~v;
        }
    }
    return term;
}

static boloq::boolean_function parse_function(blif::trimmed_lines::iterator& it) {
    using namespace boloq;
    boolean_function result = boolean_function::zero();
    while (continue_line(*it)) {
        const std::string& line = *it;
        if (line == "1") {
            result |= boolean_function::one();
        }
        else {
            result |= parse_term_line(line);
        }
        ++it;
    }
    return result;
}

gate gate::parse(blif::trimmed_lines::iterator& it) {
    std::vector<std::string> splited;
    boost::algorithm::split(splited, *it++, boost::is_space(), boost::algorithm::token_compress_on);
    assert(splited[0] == ".names");
    return gate(splited.begin() + 1, splited.end() - 1,
                *splited.rbegin(), parse_function(it));
}

}
