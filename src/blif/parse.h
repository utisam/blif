#pragma once
#include <iterator>
#include <string>
#include <unordered_map>
#include <vector>
#include <boloq/boolean_function.h>
#include <blif/gate.h>
#include <blif/circuit.h>

/*!
 * @brief BLIF をパースするためのライブラリ libblif の名前空間です
 */
namespace blif {

/*!
 * @brief BLIFをパースします
 */
std::unordered_map<std::string, circuit> parse(std::istream& ist);

}
