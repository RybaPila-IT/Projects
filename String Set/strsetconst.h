#ifndef STRSETCONST_H
#define STRSETCONST_H

#ifdef __cplusplus

    #include <iostream>

namespace jnp1 {

    extern "C" {
#endif // _cplusplus

    /** @brief Zwraca identyfikator zbioru, którego nie można modyfikować.
     * Zbiór ten zawiera jeden element: napis "42". Zbiór jest tworzony przy
     * pierwszym wywołaniu tej funkcji i wtedy zostaje ustalony jego numer.
     * @return Identyfikator ww. zbioru.
     */
    unsigned long strset42();

#ifdef __cplusplus
    };

}
#endif // _cplusplus

#endif // STRSETCONST_H