#include <type_traits>
#include <functional>

#define get [this]()
#define set [this](auto value)

template<typename T, typename is_reference = void, typename is_class = void>
struct property;

template<typename T>
struct property
<
    T,
    typename std::enable_if<!std::is_reference<T>::value>::type,
    typename std::enable_if<!std::is_class<T>::value>::type
>
{
    using property_t = property<T>;
    using getter_t = std::function<T()>;
    using setter_t = std::function<void(T)>;

    property(getter_t getter) : _getter{getter}, _setter{} {}
    property(setter_t setter) : _getter{}, _setter{setter} {}
    property(getter_t getter, setter_t setter) : _getter{getter}, _setter{setter} {}
    property(setter_t setter, getter_t getter) : _getter{getter}, _setter{setter} {}

    operator T() {
        return _getter();
    }

    template<typename V> property_t& operator=(V value)
    {
        _setter(value);
        return *this;
    }

private:
    getter_t _getter;
    setter_t _setter;
};

// operator.() not allowed. Have to use inheritence for class types.
// See https://isocpp.org/blog/2016/02/a-bit-of-background-for-the-operator-dot-proposal-bjarne-stroustrup
template<typename T>
struct property
<
    T,
    typename std::enable_if<!std::is_reference<T>::value>::type,
    typename std::enable_if<std::is_class<T>::value>::type
>
: public T
{
    using property_t = property<T>;
    using getter_t = std::function<T()>;
    using setter_t = std::function<void(const T&)>;

    property(getter_t getter) : _getter{getter}, _setter{} {}
    property(setter_t setter) : _getter{}, _setter{setter} {}
    property(getter_t getter, setter_t setter) : _getter{getter}, _setter{setter} {}
    property(setter_t setter, getter_t getter) : _getter{getter}, _setter{setter} {}

    operator T() {
        return *this;
    }

    template<typename V> property_t& operator=(const V& value)
    {
        _setter(value);
        this->T::operator=(_getter());

        return *this;
    }

private:
    getter_t _getter;
    setter_t _setter;
};

template<typename T>
struct property
<
    T&,
    typename std::enable_if<std::is_reference<T&>::value>::type,
    typename std::enable_if<!std::is_class<T>::value>::type
>
{
    using property_t = property<T&>;
    using getter_t = std::function<T()>;
    using setter_t = std::function<void(T*)>;

    property(getter_t getter) : _getter{getter}, _setter{} {}
    property(setter_t setter) : _getter{}, _setter{setter} {}
    property(getter_t getter, setter_t setter) : _getter{getter}, _setter{setter} {}
    property(setter_t setter, getter_t getter) : _getter{getter}, _setter{setter} {}

    operator T() {
        return _getter();
    }

    template<typename V> property_t& operator=(V& value)
    {
        _setter(&value);
        return *this;
    }

private:
    getter_t _getter;
    setter_t _setter;
};

template<typename T>
struct property
<
    T&,
    typename std::enable_if<std::is_reference<T&>::value>::type,
    typename std::enable_if<std::is_class<T>::value>::type
>
: public T
{
    using property_t = property<T&>;
    using getter_t = std::function<T()>;
    using setter_t = std::function<void(T*)>;

    property(getter_t getter) : _getter{getter}, _setter{} {}
    property(setter_t setter) : _getter{}, _setter{setter} {}
    property(getter_t getter, setter_t setter) : _getter{getter}, _setter{setter} {}
    property(setter_t setter, getter_t getter) : _getter{getter}, _setter{setter} {}

    operator T() {
        return _getter();
    }

    template<typename V> property_t& operator=(V& value)
    {
        _setter(&value);
        this->T::operator=(_getter());

        return *this;
    }

private:
    getter_t _getter;
    setter_t _setter;
};
