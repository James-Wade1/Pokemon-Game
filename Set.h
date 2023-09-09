#ifndef SET_H
#define SET_H

#include "BST.h"
#include "SetInterface.h"
#include <sstream>
#include <iostream>

template <typename T>
class Set : SetInterface<T> {
public:
    Set() {}
    ~Set() { clear(); }
    
    /** Inserts item into the set, if the container doesn't
        already contain an element with an equivalent value.
        @return: pair.first = pointer to item
                 pair.second = true if successfully inserted, else false. */
    //virtual Pair<T*, bool> insert(const T& item) = 0;
    virtual bool insert(const T& item) { return set.addNode(item); }

    /** Removes all items from the set. */
    virtual void clear() { set.clearTree(); }

    /** @return: the number of elements contained by the Set. */
    virtual size_t size() const { return set.size(); }

    /** @return: return 1 if contains element equivalent to item, else 0. */
    virtual size_t count(const T& item) { return ((set.find(item)) ? 1 : 0); }

    /** @return: string representation of items in Set. */
    virtual std::string toString() const {
        std::ostringstream os;
        for (typename BST<T>::Iterator iter = set.begin(); iter != set.end(); ++iter) {
            os << *iter << ",";
        }
        std::string str(os.str());
        str.pop_back();
        return str;
    }
    
    /** Friend insertion operator*/
    friend std::ostream& operator<<(std::ostream& os, Set<T>& set) {
        return os << set.toString();
    }
    
private:
    BST<T> set;
};


#endif /* SET_H */
