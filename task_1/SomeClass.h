#ifndef SOMECLASS_H
#define SOMECLASS_H

#include <memory>
#include <cstring>
#include <cassert>

/*******************************************************************************************************
 * Q: Опишите и исправьте проблемы (если они есть) в этом классе
 *
 * A: В изначальном варианте были проблемы с памятью и исключениями, а именно:
 *    - выделялась память на 2 буфера, но нигде не удалялась, явно не хватает деструктора. Утечка.
 *    - при выделении памяти в x2 может что-то пойти не так, например вылететь bad_alloc, тогда буфер,
 *      выделенный в x1 не будет удалён. Тоже будет утечка.
 *    - вообще, сырые указатели использовать надо только в крайнем случае, RAII объекты (которые умные
 *      указатели) предпочтительнее, т.к. они не только берегут от многих проблем, но и позволяют
 *      строже и чище реализовывать всяческие операторы присваивания/конструкторы
********************************************************************************************************/

/*!
 * \brief The SomeClass class
 * это исправленный SomeClass, в котором я взял на себя смелость чууть-чуть дополнить
 * его простейшим конструктором, чтобы можно было продемонстрировать его работу хоть как-то.
 * Так как это НЕ класс строки, в решении нет попыток копировать данные осмысленно.
 */
class SomeClass
{
    static constexpr unsigned N = 1024;
public:
    SomeClass()
        : x1{std::make_unique<char[]>(N)}
        , x2{std::make_unique<char[]>(N * N)}
    { }

    /*!
     * \brief SomeClass -- максимально бессмысленная инициализация строкой
     * \param init_data -- сишная строка, либо литерал
     */
    SomeClass(const std::string& init_data)
        : x1{std::make_unique<char[]>(N)}
        , x2{std::make_unique<char[]>(N * N)}
    {
        assert(init_data.length() < N);
        std::fill_n(x1.get(), N, 0);
        std::fill_n(x2.get(), N * N, 0);

        std::memcpy(x1.get(), init_data.c_str(), init_data.length());

        char* pos = x2.get();
        for (unsigned i = 0; i < N; ++i)
        {
            std::memcpy(pos, init_data.c_str(), init_data.length());
            pos += N;
        }
    }

    // good old конструктор копирования
    SomeClass(const SomeClass& other)
        : x1{std::make_unique<char[]>(N)}
        , x2{std::make_unique<char[]>(N * N)}
    {
        std::memcpy(x1.get(), other.x1.get(), N);
        std::memcpy(x2.get(), other.x2.get(), N * N);
    }

    // писать свой нет необходимости
    SomeClass(SomeClass&& other) = default;

    // это 2 в 1
    SomeClass& operator=(SomeClass other) noexcept
    {
        swap(*this, other);
        return *this;
    }

    // noexcept, потому что таковые есть для unique_ptr
    friend void swap(SomeClass& first, SomeClass& second) noexcept
    {
        using std::swap;
        swap(first.x1, second.x1);
        swap(first.x2, second.x2);
    }

    // он тут сделан для тестов, самый простой
    friend bool operator==(const SomeClass& first, const SomeClass& second)
    {
        return std::memcmp(first.x1.get(), second.x1.get(), N) == 0
                && std::memcmp(first.x2.get(), second.x2.get(), N * N) == 0;
    }

private:
    // С умными указателями можно изящнеее поддержать move & swap
    std::unique_ptr<char[]> x1;
    std::unique_ptr<char[]> x2;
};

/* Возможно, предполагалось просто написать нечто подобное тому, что закомментировано ниже, а реализацию
 * основывать на вызове delete[] в правильных местах. И весь трюк был бы в том, чтобы сначала
 * успешно выделить память, а потом уже удалять по старому указателю что-то. Но мне показалось,
 * что такую архаику можно не демонстрировать. Также в моей версии явно прописывать то, что
 * привожу ниже, избыточно и просто не нужно.
 **********************************************************************************************************/

//SomeClass(SomeClass&& other)
//    : x1{std::move(other.x1)}
//    , x2{std::move(other.x2)}
//{}

//SomeClass& operator=(const SomeClass& other)
//{
//    if (this != &other)
//    {
//        SomeClass tmp(other);
//        swap(*this, tmp);
//    }

//    return *this;
//}

//SomeClass& operator=(SomeClass&& other)
//{
//    if (this != &other)
//    {
//        x1 = std::move(other.x1);
//        x2 = std::move(other.x2);
//    }

//    return *this;
//}

#endif // SOMECLASS_H
