#include <fstream>
#include <iostream>
#include <blif/parse.h>
using namespace std;

int print_usage() {
    cerr << "Usage: blif_stat [FILE]" << endl;
    return -1;
}

int main(int argc, char* argv[]) {
    if (argc != 2) return print_usage();
    ifstream ifs(argv[1]);
    if (!ifs) return print_usage();

    bool first = true;
    for (const auto& p : blif::parse(ifs)) {
        const blif::circuit& cir = p.second;

        if (first) first = false;
        else cout << endl;
        cout << cir.name() << endl;
        cout << "inputs: " << cir.inputs().size() << endl;
        cout << "outputs: " << cir.outputs().size() << endl;
        cout << "gates: " << cir.cgates().size() << endl;
    }
    return 0;
}
