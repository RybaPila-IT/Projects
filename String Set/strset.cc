#include <string>
#include <set>
#include <unordered_map>
#include <cassert>

#include "strset.h"
#include "strsetconst.h"

namespace {

#ifdef NDEBUG
    const bool debug = true;
#else
    const bool debug = false;
#endif // NDEBUG

    using un_l = unsigned long;
    using set_s = std::set<std::string>;

    un_l nextNewId = 0;
    bool placeForNewSet = true;
    std::unordered_map<un_l, set_s>& strsets() {
        static std::unordered_map<un_l, set_s>* ans = new std::unordered_map<un_l, set_s>();

        return *ans;
    }

    bool set_exists(un_l id) {
        return strsets().find(id) != strsets().end();
    }

    bool is_set42(un_l id) {
        return id == jnp1::strset42();
    }

    bool full() {
        return nextNewId + 1 < nextNewId;
    }

    int strset_comp_for_existing_sets(un_l id1, un_l id2) {
        auto wantedSet1 = strsets().find(id1)->second, wantedSet2 = strsets().find(id2)->second;
        auto iterator1 = wantedSet1.begin(), iterator2 = wantedSet2.begin();
        std::string set1, set2;
        if (is_set42(id1))
            set1 = "the 42 Set";
        else
            set1 = "set " + std::to_string(id1);
        if (is_set42(id2))
            set2 = "the 42 Set";
        else
            set2 = "set " + std::to_string(id2);

        while (true) {
            if (iterator1 == wantedSet1.end() && iterator2 == wantedSet2.end()) {
                if (!debug)
                    std::cerr << "strset_comp: result of comparing " << set1 << " to " << set2 << " is 0" <<
                                 std::endl;

                return 0;
            }
            if (iterator1 == wantedSet1.end()) {
                if (!debug)
                    std::cerr << "strset_comp: result of comparing " << set1 << " to " << set2 << " is -1" <<
                                 std::endl;

                return -1;
            }
            if (iterator2 == wantedSet2.end()) {
                if (!debug)
                    std::cerr << "strset_comp: result of comparing " << set1 << " to " << set2<< " is 1" <<
                                 std::endl;

                return 1;
            }

            int compareResult = (*iterator1).compare(*iterator2);
            if (compareResult < 0) {
                if (!debug)
                    std::cerr << "strset_comp: result of comparing " << set1 << " to " << set2 << " is -1" <<
                                 std::endl;

                return -1;
            }
            if (compareResult > 0) {
                if (!debug)
                    std::cerr << "strset_comp: result of comparing " << set1 << " to " << set2 << " is 1" <<
                                 std::endl;

                return 1;
            }
            iterator1++;
            iterator2++;
        }
    }

} // End of namespace.

namespace jnp1 {

    unsigned long strset_new() {
        if (!debug) {
            std::cerr << "strset_new()" << std::endl;
            assert(placeForNewSet);
        }

        set_s empty;
        strsets().insert(make_pair(nextNewId, empty));

        if (full())
            placeForNewSet = false;
        nextNewId++;

        if (!debug)
            std::cerr << "strset_new: set " << nextNewId - 1 << " created" << std::endl;

        return nextNewId - 1;
    }

    void strset_delete(unsigned long id) {
        if (!debug)
            std::cerr << "strset_delete(" << id << ")" << std::endl;

        if (is_set42(id)) {
            if (!debug)
                std::cerr << "strset_delete: attempt to remove the 42 Set" << std::endl;

            return;
        }

        if (!set_exists(id)) {
            if (!debug)
                std::cerr << "strset_delete: set " << id << " does not exist" << std::endl;

            return;
        }

        strsets().erase(id);
        if (!debug)
            std::cerr << "strset_delete: set " << id << " deleted" << std::endl;
    }

    size_t strset_size(unsigned long id) {
        std::string set;
        if (is_set42(id))
            set = "the 42 Set";
        else
            set = "set " + std::to_string(id);

        if (!debug)
            std::cerr << "strset_size(" << id << ")" << std::endl;

        if (!set_exists(id)) {
            if (!debug)
                std::cerr << "strset_size: " << set << " does not exist" << std::endl;

            return 0;
        }

        size_t size = strsets().find(id)->second.size();

        if (!debug)
            std::cerr << "strset_size: " << set << " contains " << size << " element(s)" << std::endl;

        return size;
    }

    void strset_insert(unsigned long id, const char *value) {
        std::string newValue;
        if (value == nullptr)
            newValue = "NULL";
        else
            newValue = "\"" + std::string(value) + "\"";

        if (!debug)
            std::cerr << "strset_insert(" << id << ", " << newValue << ")" << std::endl;

        if (is_set42(id)) {
            if (!debug)
                std::cerr << "strset_insert: attempt to insert into the 42 Set" << std::endl;

            return;
        }

        if (value == nullptr) {
            if (!debug)
                std::cerr << "strset_insert: invalid value (NULL)" << std::endl;

            return;
        }

        if (!set_exists(id)) {
            if (!debug)
                std::cerr << "strset_insert: set " << id << " does not exist" << std::endl;

            return;
        }

        auto wantedSet = strsets().find(id);

        if (wantedSet->second.count(newValue) == 0) {
            wantedSet->second.insert(newValue);
            if (!debug)
                std::cerr << "strset_insert: set " << id << ", element " << newValue << " inserted" << std::endl;
        }
        else if (!debug)
                std::cerr << "strset_insert: set " << id << ", element " <<
                        newValue << " was already present" << std::endl;
    }

    void strset_remove(unsigned long id, const char *value) {
        std::string newValue;
        if (value == nullptr)
            newValue = "NULL";
        else
            newValue = "\"" + std::string(value) + "\"";

        if (!debug)
            std::cerr << "strset_remove(" << id << ", " << newValue << ")" << std::endl;

        if (is_set42(id)) {
            if (!debug)
                std::cerr << "strset_remove: attempt to remove from the 42 Set" << std::endl;

            return;
        }

        if (value == nullptr) {
            if (!debug)
                std::cerr << "strset_remove: invalid value (NULL)" << std::endl;

            return;
        }

        if (!set_exists(id)) {
            if (!debug)
                std::cerr << "strset_remove: set " << id << " does not exist" << std::endl;

            return;
        }

        auto wantedSet = strsets().find(id);

        if (wantedSet->second.count(newValue) == 1) {
            if (!debug)
                std::cerr << "strset_remove: set " << id << ", element " << newValue <<
                     " removed" << std::endl;

            wantedSet->second.erase(newValue);
        }
        else if (!debug)
            std::cerr << "strset_remove: set " << id << " does not contain the element " <<
                 newValue << std::endl;
    }

    int strset_test(unsigned long id, const char *value) {
        std::string newValue, set;
        if (value == nullptr)
            newValue = "NULL";
        else
            newValue = "\"" + std::string(value) + "\"";
        if (is_set42(id))
            set = "the 42 Set";
        else
            set = "set " + std::to_string(id);

        if (!debug)
            std::cerr << "strset_test(" << id << ", "<< newValue << ")" << std::endl;

        if (value == nullptr) {
            if (!debug)
                std::cerr << "strset_test: invalid value (NULL)" << std::endl;

            return 0;
        }

        if (!set_exists(id)) {
            if (!debug)
                std::cerr << "strset_test: set " << id << " does not exist" << std::endl;

            return 0;
        }

        auto wantedSet = strsets().find(id);

        if (wantedSet->second.count(newValue) == 0) {
            if (!debug)
                std::cerr << "strset_test: " << set << " does not contain the element " << newValue << std::endl;

            return 0;
        }
        if (!debug)
            std::cerr << "strset_test: " << set << " contains the element " << newValue << std::endl;

        return 1;
    }

    void strset_clear(unsigned long id) {
        if (!debug)
            std::cerr << "strset_clear(" << id << ')' << std::endl;

        if (is_set42(id)) {
            if (!debug)
                std::cerr << "strset_clear: attempt to clear the 42 Set" << std::endl;

            return;
        }

        if (!set_exists(id)) {
            if (!debug)
                std::cerr << "strset_clear: set " << id << " does not exist" << std::endl;

            return;
        }

        auto wantedSet = strsets().find(id);
        wantedSet->second.clear();
        if (!debug)
            std::cerr << "strset_clear: set " << id << " cleared" << std::endl;
    }

    int strset_comp(unsigned long id1, unsigned long id2) {
        if (!debug)
            std::cerr << "strset_comp(" << id1 << ", " << id2 << ")" << std::endl;

        bool set1Exists = set_exists(id1), set2Exists = set_exists(id2);
        std::string set1, set2;
        if (is_set42(id1))
            set1 = "the 42 Set";
        else
            set1 = "set " + std::to_string(id1);
        if (is_set42(id2))
            set2 = "the 42 Set";
        else
            set2 = "set " + std::to_string(id2);

        if (!set1Exists && !set2Exists) {
            if (!debug)
                std::cerr << "strset_comp: result of comparing " << set1 << " to " << set2 << " is 0" <<
                             std::endl << "strset_comp: " << set1 << " does not exist" << std::endl <<
                             "strset_comp: " << set2 << " does not exist" << std::endl;

            return 0;
        }
        if (!set1Exists) {
            if (!debug)
                std::cerr << "strset_comp: result of comparing " << set1 << " to " << set2 << " is -1" <<
                             std::endl << "strset_comp: " << set1 << " does not exist" << std::endl;

            return -1;
        }
        if (!set2Exists) {
            if (!debug)
                std::cerr << "strset_comp: result of comparing " << set1 << " to " << set2 << " is 1" <<
                             std::endl << "strset_comp: " << set2 << " does not exist" << std::endl;

            return 1;
        }

        return strset_comp_for_existing_sets(id1, id2);
    }

} // End of namespace jnp1.
