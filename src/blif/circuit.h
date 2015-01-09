#pragma once
#include <blif/details/trimmed_lines.h>

namespace blif {

/*!
 * @brief パースした回路を表すクラスです
 */
class circuit {
    std::string _name;
    std::unordered_map<std::string, gate> _gates;
    std::vector<std::string> _inputs, _outputs;

public:
    /*!
     * @brief ".model" コマンドをパースします
     */
    static circuit parse(blif::trimmed_lines::iterator&);

    /*! @brief コンストラクタ */
    circuit() = default;
    /*! @brief コピーコンストラクタ */
    circuit(const circuit&) = default;
    /*! @brief 代入演算子 */
    circuit& operator=(const circuit&) = default;

    /*! @brief コンストラクタ */
    template<class IItr, class OItr>
    circuit(const std::string& n, const std::unordered_map<std::string, gate>& g,
            const IItr& ifirst, const IItr& ilast,
            const OItr& ofirst, const OItr& olast) :
            _name(n), _gates(g), _inputs(ifirst, ilast), _outputs(ofirst, olast) {}

    /*! @brief 回路の名前を取得します */
    const std::string& name() const {
        return _name;
    }

    /*! @brief ゲートの集合を取得します */
    const std::unordered_map<std::string, gate>& cgates() const {
        return _gates;
    }
    /*! @brief ゲートの集合を取得します（変更可能版） */
    std::unordered_map<std::string, gate>& gates() {
        return _gates;
    }

    /*! @brief 入力線のラベルを取得します */
    const std::vector<std::string>& inputs() const {
        return _inputs;
    }

    /*! @brief 出力線のラベルを取得します */
    const std::vector<std::string>& outputs() const {
        return _outputs;
    }

    void insert(const gate& g) {
        _gates.emplace(g.output(), g);
    }

    void erase(const gate& g) {
        _gates.erase(g.output());
    }
};

}
