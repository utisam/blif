#include <blif/parse.h>

namespace blif {

std::unordered_map<std::string, circuit> parse(std::istream& ist) {
    std::unordered_map<std::string, circuit> result;
    blif::trimmed_lines reader(ist);
    for (auto it = reader.begin(); it != reader.end(); ++it) {
        circuit model = circuit::parse(it);
        result.insert({model.name(), model});
    }
    return result;
}

}
