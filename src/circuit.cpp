#include <cassert>
#include <cstdlib>
#include <exception>
#include <iostream>
#include <string>
#include <vector>
#include <boost/algorithm/string.hpp>
#include <boloq/boolean_function.h>
#include <blif/details/trimmed_lines.h>
#include <blif/gate.h>
#include <blif/circuit.h>

namespace blif {

circuit circuit::parse(blif::trimmed_lines::iterator& it) {
    assert(boost::algorithm::starts_with(*it, ".model "));
    const std::string name = it->substr(7);
    ++it;

    std::vector<std::string> inputs;
    std::vector<std::string> outputs;
    std::unordered_map<std::string, gate> gates;

    const std::unordered_map<std::string, std::function<void()>> str_switch = {{
        {".inputs", [&]() {
            boost::algorithm::split(inputs, *it++, boost::is_space(), boost::algorithm::token_compress_on);
        }},
        {".outputs", [&]() {
            boost::algorithm::split(outputs, *it++, boost::is_space(), boost::algorithm::token_compress_on);
        }},
        {".names", [&]() {
            gate g = gate::parse(it);
            gates.insert({g.output(), g});
        }},
    }};

    while (*it != ".end") {
        const std::string& line = *it;
        size_t pos = line.find(' ');
        assert(pos != std::string::npos);
        const std::string command = line.substr(0, pos);

        if (str_switch.count(command)) {
            str_switch.at(command)();
        }
        else {
            throw std::runtime_error("unknown command: " + command);
            ++it;
        }
    }
    ++it;

    return circuit(name, gates,
                   inputs.begin() + 1, inputs.end(),
                   outputs.begin() + 1, outputs.end());
}

}
