#ifndef STRIPEDHASHMAP_H
#define STRIPEDHASHMAP_H

#include <functional>
#include <list>
#include <set>
#include <vector>
#include <iterator>

#include <mutex>
#include <shared_mutex>
#include <atomic>

using RWMutex = std::shared_mutex;
//using RWMutex = std::shared_timed_mutex; // Если нет C++17

/* StackOverflow хорош как всегда, помогает с такими  небольшими
 * приятными трюками. Если размер не будет степенью двойки, то будут проблемы,
 * также как и если расширять вектор не в целое число раз (тогда ещё большие)
 **********************************************************************************/
namespace utils
{

constexpr bool isPowerOf2(size_t value){
    return !(value == 0) && !(value & (value - 1));
}

template <bool...> struct bool_pack{};
template <bool... bools>
struct conjunction : std::is_same<bool_pack<true, bools...>,
        bool_pack<bools..., true>>
{};

template <size_t... Args>
constexpr bool arePowerOf2() {
    return conjunction<isPowerOf2(Args)...>::value;
}

}

template <typename KeyType, typename ValueType, std::size_t InitialSize = 256>
class StripedHashMap
{
    using PairType   = std::pair<KeyType, ValueType>;
    using BucketType = std::list<PairType>;

    static constexpr float MAX_LOAD_FACTOR = 1.5; // ГОВОРЯТ, 1.5-2 это оптимальное значение.
    static_assert(utils::arePowerOf2<InitialSize>(), "StripedHashMap: argument InitialSize must be a power of 2");

public:
    void insert(KeyType key, ValueType value)
    {
        if (contains(key))
            return;  // RET

        {
            std::lock_guard<RWMutex> locker{m_locks[l_index(key)]}; // std::scoped_lock с C++17
            std::lock_guard<RWMutex> keys_locker{m_key_mutex};
            m_keys.insert(key);
            m_data[d_index(key)].emplace_back(
                        std::make_pair(std::move(key), std::move(value)));
            ++m_size;
        }

        if (load_factor() > MAX_LOAD_FACTOR)
            rehash();
    }

    void replace(KeyType key, ValueType value)
    {
        std::lock_guard<RWMutex> locker{m_locks[l_index(key)]}; // std::scoped_lock с C++17
        replace_impl(std::make_pair(std::move(key), std::move(value)), d_index(key));
    }

    ValueType value(const KeyType& key) const
    {
        std::shared_lock<RWMutex> locker{m_locks[l_index(key)]};

        const BucketType& bucket = m_data[d_index(key)];
        for(const auto& item : bucket)
        {
            if (item.first == key)
                return item.second;
        }

        return ValueType();
    }

    bool contains(const KeyType& key) const
    {
        std::shared_lock<RWMutex> locker(m_locks[l_index(key)]);

        bool result = false;
        const BucketType& bucket = m_data[d_index(key)];
        for (const auto& item : bucket)
        {
            if (item.first == key)
            {
                result = true;
                break;
            }
        }
        return result;
    }

    // имеет смысл скорее для отладки
    int size() const
    {
        return m_size;
    }

    std::list<KeyType> keys() const
    {
        // т.к. нужно копировать ключи, пришлось добавить мьютекс
        std::shared_lock<RWMutex> keys_locker(m_key_mutex);

        std::list<KeyType> result;
        std::copy(m_keys.begin(), m_keys.end(), std::back_inserter(result));
        return result;
    }

private:
    void rehash()
    {
        std::lock_guard<std::mutex> locker{m_rehash_mutex};

        // double-checked locking
        if (load_factor() <= MAX_LOAD_FACTOR)
            return; // RET

        std::vector<std::unique_lock<std::shared_mutex>> all_locks;
        for (unsigned i = 0; i < m_locks.size(); ++i)
            all_locks.emplace_back(m_locks[i]);

        static const int REALLOC_FACTOR = 2;
        std::vector<BucketType> new_data(REALLOC_FACTOR * m_data.size());
        m_data.swap(new_data);

        for (BucketType& bucket : new_data)
        {
            for(auto it = bucket.begin(); it != bucket.end(); ++it)
            {
                m_data[d_index(it->first)].emplace_back(*std::make_move_iterator(it));
            }
        }
    }

    void replace_impl(PairType&& item, std::size_t index)
    {
        BucketType& bucket = m_data[index];
        for(auto it = bucket.begin(); it != bucket.end(); ++it)
        {
            if (it->first == item.first)
                *it = std::move(item);
        }
    }

    std::size_t l_index(const KeyType& key) const
    {
        return Hasher(key) % m_locks.size();
    }

    std::size_t d_index(const KeyType& key) const
    {
        return Hasher(key) % m_data.size();
    }

    float load_factor()
    {
        return static_cast<float>(m_size) / m_data.size();
    }

private:
    mutable std::vector<RWMutex>  m_locks = std::vector<RWMutex>(InitialSize);
    std::vector<BucketType>       m_data  = std::vector<BucketType>(InitialSize);
    std::atomic_size_t            m_size{0};

    std::set<KeyType>             m_keys;
    mutable RWMutex               m_key_mutex;
    std::mutex                    m_rehash_mutex;

    static const std::function<std::size_t(const KeyType&)> Hasher;
};

template <typename KeyType, typename ValueType, std::size_t InitialSize>
const std::function<std::size_t(const KeyType&)> StripedHashMap<KeyType, ValueType, InitialSize>::Hasher = std::hash<KeyType>{};

#endif // STRIPEDHASHMAP_H
