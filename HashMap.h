#ifndef HASHMAP_H
#define HASHMAP_H

#include <utility>
#include <cmath>
#include "HashMapInterface.h"

template <typename K, typename V>
class HashMap : HashMapInterface<K, V> {
public:
    HashMap(size_t size = DEFAULT_MAP_HASH_TABLE_SIZE) : capacity(size), numKeys(0), numDeleted(0) {
        hashMap = new EntryType*[size];
        for (unsigned int i = 0; i < size; i++) {
            hashMap[i] = NULL;
        }
    }
    HashMap(const HashMap<K,V>& oldHash) : capacity(oldHash.size()), numKeys(0) {
        hashMap = new EntryType**[oldHash.size()];
        for (unsigned int i = 0; i < oldHash.size(); i++) {
            if (oldHash[i] != NULL && oldHash[i] != DELETED) {
                hashMap[oldHash[i]->first] = oldHash[i]->second;
            }
        }
    }
    ~HashMap() { clear(); }
    
    /**Defines the Type of Pointers in Array */
    typedef std::pair<const K, V> EntryType;
    
    /** Read/write index access operator.
    If the key is not found, an entry is made for it.
    @return: Read and write access to the value mapped to the provided key. */
    virtual V& operator[](const K& key) {
        std::pair<size_t, bool> node = insert(EntryType(key, V()));
        return hashMap[node.first]->second;
    }

    /** @return: the number of elements that match the key in the Map. */
    virtual size_t count(const K& key);

    /** Removes all items from the Map. */
    virtual void clear() {
        for (unsigned int i = 0; i < capacity; i++) {
            if (hashMap[i] != NULL) {
                delete hashMap[i];
            }
        }
        delete[] hashMap;
    }

    /** @return: number of Key-Value pairs stored in the Map. */
    virtual size_t size() const { return numKeys; }

    /** @return: maximum number of Key-Value pairs that the Map can hold. */
    virtual size_t max_size() const {return capacity; }

    /** @return: string representation of Key-Value pairs in Map. */
    virtual std::string toString() const;
    
    /** Friend insertion operator*/
    friend std::ostream& operator<<(std::ostream& os, HashMap<K,V>& hashMap) {
        return os << hashMap.toString();
    }
    
private:
    EntryType** hashMap;
    size_t capacity;
    size_t numKeys;
    size_t numDeleted;
    
    /** Inserts a key if not present and returns index, or returns the index for existing key */
    std::pair<size_t, bool> insert(const EntryType& entry);
    
    /** Removes key from find function by delcaring it "DELETED"*/
    void remove(const K& key) {
        size_t index = find(key);
        if (hashMap[index] != NULL) {
            hashMap[index] == DELETED;
            numKeys--;
            numDeleted++;
        }
    }
    
    /** Uses the Hash Index to find if there is an occurrence of the key, and if not, it gives the next open spot */
    size_t find(const K& key, EntryType** hashMap) {
        size_t index = hashIndex(key);
        int k = -1;
        while (hashMap[index] != NULL && (hashMap[index] == DELETED || hashMap[index]->first != key)) {
            k += 2;
            index = (index + k) % capacity;
        }
        return index;
    }
    
    /** Finds the Hash Index for the key */
    size_t hashIndex(const K& key) {
        long long hashCode = 0;
        size_t size(key.size());
        for (unsigned int i = 0; i < size; i++) {
            hashCode += key.at(i) * std::pow(HASH_CONSTANT, size - (i + 1));
        }
        size_t hashIndex = hashCode % capacity;
        return hashIndex;
    }
    
    /** Resize HashMap */
    void rehash();
    
    /** To show if an item has been deleted or not */
    static EntryType dummy;
    static EntryType* const DELETED;
};

template <typename K, typename V>
typename HashMap<K,V>::EntryType HashMap<K,V>::dummy = std::pair<const K,V>(K(), V());

template <typename K, typename V>
typename HashMap<K,V>::EntryType* const HashMap<K,V>::DELETED = &HashMap<K,V>::dummy;

/** @return: the number of elements that match the key in the Map. */
template <typename K, typename V>
size_t HashMap<K,V>::count(const K& key) {
    size_t count = 0;
    size_t index = find(key, hashMap);
    int k = -1;
    while (hashMap[index] != NULL) {
        count++;
        k += 2;
        index = (index + k) % capacity;
    }
    return count;
}


/** @return: string representation of Key-Value pairs in Map. */
template <typename K, typename V>
std::string HashMap<K,V>::toString() const {
    std::ostringstream os;
    os << ": " << numKeys << "/" << capacity << std::endl;
    for (unsigned int i = 0; i < capacity; i++) {
        if (hashMap[i] != NULL && hashMap[i] != DELETED) {
            os << "  [" << i << ":" << hashMap[i]->first << "->" << hashMap[i]->second << "]" << std::endl;
        }
    }
    return os.str();
}

/** Inserts a key if not present and returns index, or returns the index for existing key */
template <typename K, typename V>
std::pair<size_t, bool> HashMap<K,V>::insert(const EntryType& entry) {
    double load = 100 * double(numKeys + numDeleted) / capacity;
    if (load > LOAD_THRESHOLD) {
        rehash();
    }
    size_t index = find(entry.first, hashMap);
    if (hashMap[index] == NULL) {
        hashMap[index] = new EntryType(entry);
        numKeys++;
        return std::make_pair(index, true);
    }
    else {
        return std::make_pair(index, false);
    }
}

/** Resize HashMap */
template <typename K, typename V>
void HashMap<K,V>::rehash() {
    size_t newCapacity = 2 * capacity;
    EntryType** oldHashMap = new EntryType*[newCapacity];
    for (unsigned int i = 0; i < newCapacity; i++) {
        oldHashMap[i] = NULL;
    }
    size_t oldCapacity = capacity;
    capacity = newCapacity;
    for (unsigned int i = 0; i < oldCapacity; i++) {
        if (hashMap[i] != NULL && hashMap[i] != DELETED) {
            size_t index = find(hashMap[i]->first, oldHashMap);
            oldHashMap[index] = hashMap[i];
        }
    }
    std::swap(hashMap, oldHashMap);
    delete[] oldHashMap;
    numDeleted = 0;
}

#endif /* HASHMAP_H */
