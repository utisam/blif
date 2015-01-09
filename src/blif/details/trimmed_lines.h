#pragma once

namespace blif {

/*!
 * @brief コメントや余分な空白を取り除きます
 *
 * バックスラッシュによる行の連結も行います
 */
class trimmed_lines {
private:
    std::istream& ist;

public:

    /*!
     * @brief 各行を扱う input_iterator です
     */
    class iterator
        : public std::iterator<
            std::input_iterator_tag,
            const std::string>
    {
    private:
        friend trimmed_lines;

        std::istream* pi;
        std::string line;

        iterator() : pi(nullptr) {}
        iterator(std::istream& ist) : pi(&ist) {++*this;}

        bool nextline();

    public:

        /*! @brief 現在の行を取得します */
        reference operator*() const {return line;}

        /*! @brief 現在の行を操作します */
        pointer operator->() const {return &line;}

        /*! @brief 次の行に進みます */
        iterator operator++() {
            if (pi && !nextline()) pi = nullptr;
            return *this;
        }

        /*! @brief 次の行に進みます */
        iterator operator++(int) {
            iterator tmp = *this;
            ++*this;
            return tmp;
        }

        /*!
         * @brief 末尾ではないかどうかを判定するためのみに使用します
         *
         * 入力元が違うかどうかを判定しています
         */
        bool operator!=(const iterator& o) const {
            return pi != o.pi;
        }

        /*!
         * @brief 末尾かどうかを判定するためのみに使用します
         *
         * 入力元が同じかどうかを判定しています
         */
        bool operator==(const iterator& o) const {
            return pi == o.pi;
        }

    };

    /*! @brief コンストラクタ */
    explicit trimmed_lines(std::istream& _ist) : ist(_ist) {}

    /*! @brief イテレーションを開始します */
    iterator begin() {
        return iterator(ist);
    }

    /*! @brief 末尾のイテレータを取得します */
    iterator end() {
        return iterator();
    }
};

}
