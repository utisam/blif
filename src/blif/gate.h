#pragma once
#include <blif/details/trimmed_lines.h>

namespace blif {

/*!
 * @brief パースしたゲートを表すクラスです
 */
class gate {
private:
    std::vector<std::string> _inputs;
    std::string _output;
    boloq::boolean_function _logic;

public:
    /*! @brief ".names" コマンドをパースします */
    static gate parse(blif::trimmed_lines::iterator&);

    /*! @brief コンストラクタ */
    gate() = default;
    /*! @brief コピーコンストラクタ */
    gate(const gate&) = default;
    /*! @brief 代入演算子 */
    gate& operator=(const gate&) = default;

    /*! @brief コンストラクタ */
    template<class Itr>
    gate(const Itr& first, const Itr& last, const std::string& _o,
         const boloq::boolean_function& f) :
        _inputs(first, last), _output(_o),
        _logic(f)
    {}

    /*! @brief 入力線のラベルを取得します */
    const std::vector<std::string>& inputs() const {
        return _inputs;
    }

    /*! @brief 出力線のラベルを取得します */
    const std::string& output() const {
        return _output;
    }

    /*! @brief このゲートの論理を取得します */
    const boloq::boolean_function& logic() const {
        return _logic;
    }
};

}
