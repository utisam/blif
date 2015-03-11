#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <iostream>
#include <sstream>
#include <boost/test/unit_test.hpp>
#include <blif/parse.h>
#include <blif/remodeler.h>
#include <boloq.h>
#include <boloq/io.h>
using namespace std;

BOOST_AUTO_TEST_SUITE(trimed_lines_test);

BOOST_AUTO_TEST_CASE(test_reader) {
    std::istringstream ist(
        "# comment\n"
        "abc # comment\n"
        "continue \\\n"
        "line\n"
        "continue\\\n"
        " line2\n"
        "comment and # continue\\\n"
        "line\n"
        "continue line \\#and\n"
        "comment"
    );
    blif::trimmed_lines reader(ist);

    blif::trimmed_lines::iterator it = reader.begin();

    BOOST_REQUIRE_EQUAL(*it++, "abc");
    BOOST_REQUIRE_EQUAL(*it, "continue line"); ++it;
    BOOST_REQUIRE_EQUAL(*it, "continue line2"); ++it;
    BOOST_REQUIRE_EQUAL(*it, "comment and"); it++;
    BOOST_REQUIRE_EQUAL(*it, "line"); it++;
    BOOST_REQUIRE(it != reader.end());
    BOOST_REQUIRE_EQUAL(*it, "continue line comment"); ++it;
    BOOST_REQUIRE(it == reader.end());
}

BOOST_AUTO_TEST_SUITE_END();

BOOST_AUTO_TEST_SUITE(gate_parse_test);

BOOST_AUTO_TEST_CASE(test_parse) {
    std::istringstream ist(
        ".names a b  c\n"
        "11 1\n"
    );
    blif::trimmed_lines reader(ist);
    auto it = reader.begin();
    blif::gate gate = blif::gate::parse(it);

    const std::vector<std::string>& inputs = gate.inputs();
    const std::array<std::string, 2> expected_inputs = {{
        "a", "b",
    }};
    BOOST_REQUIRE_EQUAL_COLLECTIONS(
        inputs.begin(), inputs.end(),
        expected_inputs.begin(), expected_inputs.end());

    BOOST_REQUIRE_EQUAL(gate.output(), "c");

    for (size_t i = 0; i < (1U << 2); ++i) {
        std::array<bool, 2> assign;
        for (size_t j = 0; j < 2; j++) {
            assign[j] = (i & (1U << j));
        }
        BOOST_REQUIRE_EQUAL(gate.logic().execute(assign), assign[0] && assign[1]);
    }

    BOOST_REQUIRE(it == reader.end());
}

BOOST_AUTO_TEST_SUITE_END();

BOOST_AUTO_TEST_SUITE(circuit_parse_test);

BOOST_AUTO_TEST_CASE(test_parse) {
    std::istringstream ist(
        ".model model_name\n"
        ".inputs a b\n"
        ".outputs c d\n"
        ".end"
    );
    blif::trimmed_lines reader(ist);
    auto it = reader.begin();
    blif::circuit circuit = blif::circuit::parse(it);

    BOOST_REQUIRE_EQUAL(circuit.name(), "model_name");

    const std::vector<std::string>& inputs = circuit.inputs();
    const std::array<std::string, 2> expected_inputs = {{
        "a", "b",
    }};
    BOOST_REQUIRE_EQUAL_COLLECTIONS(
        inputs.begin(), inputs.end(),
        expected_inputs.begin(), expected_inputs.end());

    const std::vector<std::string>& outputs = circuit.outputs();
    const std::array<std::string, 2> expected_outputs = {{
        "c", "d",
    }};
    BOOST_REQUIRE_EQUAL_COLLECTIONS(
        outputs.begin(), outputs.end(),
        expected_outputs.begin(), expected_outputs.end());

    BOOST_REQUIRE(it == reader.end());
}

BOOST_AUTO_TEST_SUITE_END();

BOOST_AUTO_TEST_SUITE(parse_test);

BOOST_AUTO_TEST_CASE(test_parse) {
    std::istringstream ist(
        ".model model_name\n"
        ".inputs a b\n"
        ".outputs c d\n"
        ".names a b c\n"
        "11 1\n"
        ".names a b d\n"
        "10 1\n"
        "01 1\n"
        ".end"
    );
    auto circuits = blif::parse(ist);

    const blif::circuit& circuit = circuits["model_name"];
    BOOST_REQUIRE_EQUAL(circuit.name(), "model_name");

    const std::vector<std::string>& inputs = circuit.inputs();
    const std::array<std::string, 2> expected_inputs = {{
        "a", "b",
    }};
    BOOST_REQUIRE_EQUAL_COLLECTIONS(
        inputs.begin(), inputs.end(),
        expected_inputs.begin(), expected_inputs.end());

    const std::vector<std::string>& outputs = circuit.outputs();
    const std::array<std::string, 2> expected_outputs = {{
        "c", "d",
    }};
    BOOST_REQUIRE_EQUAL_COLLECTIONS(
        outputs.begin(), outputs.end(),
        expected_outputs.begin(), expected_outputs.end());

    const blif::gate& gate_c = circuit.cgates().at("c");
    for (size_t i = 0; i < (1U << 2); ++i) {
        std::array<bool, 2> assign;
        for (size_t j = 0; j < 2; j++) {
            assign[j] = (i & (1U << j));
        }
        BOOST_REQUIRE_EQUAL(gate_c.logic().execute(assign), assign[0] && assign[1]);
    }

    const blif::gate& gate_d = circuit.cgates().at("d");
    for (size_t i = 0; i < (1U << 2); ++i) {
        std::array<bool, 2> assign;
        for (size_t j = 0; j < 2; j++) {
            assign[j] = (i & (1U << j));
        }
        BOOST_REQUIRE_EQUAL(gate_d.logic().execute(assign), assign[0] ^ assign[1]);
    }

    BOOST_REQUIRE(!ist);
}

BOOST_AUTO_TEST_SUITE_END();

BOOST_AUTO_TEST_SUITE(remodeler_test);

BOOST_AUTO_TEST_CASE(test_replace_2input) {
    std::istringstream ist(
        ".model model_name\n"
        ".inputs a b c d\n"
        ".outputs e\n"
        ".names a b c d e\n"
        "1111 1\n"
        ".end"
    );
    blif::circuit cir = blif::parse(ist)["model_name"];
    blif::replace_2input(cir);
    BOOST_REQUIRE_EQUAL(cir.gates().size(), 3);
    for (const std::pair<std::string, blif::gate>& p : cir.gates()) {
        const blif::gate& g = p.second;
        cerr << g.output() << endl;
        if (g.output().size() != 1 || g.output() == "e") {
            BOOST_REQUIRE_EQUAL(g.inputs().size(), 2);
        }
        else {
            BOOST_REQUIRE_EQUAL(g.inputs().size(), 1);
        }
    }
}

BOOST_AUTO_TEST_SUITE_END()
