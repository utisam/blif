#include <forward_list>
#include <istream>
#include <unordered_map>
#include <vector>
#include <boloq/boolean_function.h>
#include <blif/gate.h>
#include <blif/circuit.h>
#include <blif/remodeler.h>
using namespace std;
using namespace boloq;

namespace blif {

static boolean_function get_2in(const boolean_function& fn) {
    boolean_function a(0), b(1);
    if (fn.is_conjunction()) return a & b;
    else if (fn.is_disjunction()) return a | b;
    else if (fn.is_exclusive_disjunction()) return a ^ b;
    throw runtime_error("the function must be or/and/xor!");
}

void replace_2input(circuit& cir) {
    vector<gate> new_gates;
    auto& gates = cir.gates();
    for (auto it = gates.begin(); it != gates.end(); ++it) {
        const gate g = it->second;
        const auto& inputs = g.inputs();
        if (inputs.size() < 3) {
            continue;
        }

        boolean_function fn = get_2in(g.logic());
        forward_list<string> wires(inputs.begin(), inputs.end());
        size_t wires_size = inputs.size(); // forward_list は size() が無いので独自に数える
        size_t gate_cnt = 0;
        while (wires_size > 1) {
            forward_list<string>::iterator cit = wires.begin(), nit = next(cit);
            while(true) {
                string new_output = g.output();
                if (wires_size != 2) {
                    new_output += "__sub" + to_string(gate_cnt++);
                }
                new_gates.emplace_back(cit, next(nit), new_output, fn);

                *cit = new_output;
                cit = wires.erase_after(cit);
                --wires_size;
                if (cit == wires.end()) break;
                nit = next(cit);
                if (nit == wires.end()) break;
            }
        }
    }
    for (const auto& g : new_gates) gates[g.output()] = g;
}

}
