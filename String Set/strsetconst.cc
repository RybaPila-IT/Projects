#include "strsetconst.h"
#include "strset.h"

namespace {

#ifdef NDEBUG
    const bool debug = true;
#else
    const bool debug = false;
#endif // NDEBUG

    unsigned long id;
    bool isInit = false;
    bool isCreatedButEmpty = false;

}

namespace jnp1 {

    unsigned long strset42() {
        if (!isInit) {
            if (isCreatedButEmpty)
                return id + 1;

            if (!debug)
                std::cerr << "strsetconst init invoked" << std::endl;

            id = strset_new();
            isCreatedButEmpty = true;
            strset_insert(id, "42");
            isInit = true;

            if (!debug)
                std::cerr << "strsetconst init finished" << std::endl;
        }

        return id;
    }

}
