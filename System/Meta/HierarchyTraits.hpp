#pragma once

typedef char yes_type;
struct no_type { char dummy[2]; };

// Teste: se Derived* pode ser convertido implicitamente para Base*
template <typename Base, typename Derived>
yes_type test_convertible(Base*);

template <typename Base, typename Derived>
no_type test_convertible(...);

template <typename Base, typename Derived>
struct is_base_of
{
    // Remove cv-qualifiers manualmente (sem std::remove_cv)
    using B = const Base;
    using D = const Derived;

    static constexpr bool value =
        sizeof(test_convertible<B, D>(static_cast<D*>(nullptr))) == sizeof(yes_type);
};

template <typename Base, typename Derived>
constexpr bool is_base_of_v = is_base_of<Base, Derived>::value;