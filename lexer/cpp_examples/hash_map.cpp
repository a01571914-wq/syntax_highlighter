#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <algorithm>
#include <optional>
using namespace std;

// ============================================================
//  Hash Map - Custom implementation with chaining, dynamic
//  resizing, and a basic open-addressing variant for comparison
// ============================================================

// Hash map using separate chaining with std::list buckets and
// automatic resizing once the load factor gets too high.
template <typename K, typename V>
class HashMap {
private:
    vector<list<pair<K, V>>> table;
    size_t count = 0;
    static constexpr double MAX_LOAD_FACTOR = 0.75;

    size_t hashKey(const K& key) const {
        return hash<K>{}(key) % table.size();
    }

    void resize() {
        size_t newSize = table.size() * 2;
        vector<list<pair<K, V>>> newTable(newSize);

        for (auto& bucket : table) {
            for (auto& kv : bucket) {
                size_t idx = hash<K>{}(kv.first) % newSize;
                newTable[idx].push_back(move(kv));
            }
        }
        table = move(newTable);
    }

public:
    explicit HashMap(size_t initialSize = 16) : table(initialSize) {}

    void put(const K& key, const V& value) {
        if ((double)(count + 1) / table.size() > MAX_LOAD_FACTOR)
            resize();

        size_t idx = hashKey(key);
        for (auto& kv : table[idx]) {
            if (kv.first == key) {
                kv.second = value;
                return;
            }
        }
        table[idx].push_back({key, value});
        count++;
    }

    optional<V> get(const K& key) const {
        size_t idx = hashKey(key);
        for (const auto& kv : table[idx])
            if (kv.first == key) return kv.second;
        return nullopt;
    }

    bool contains(const K& key) const {
        return get(key).has_value();
    }

    bool remove(const K& key) {
        size_t idx = hashKey(key);
        auto& bucket = table[idx];
        for (auto it = bucket.begin(); it != bucket.end(); ++it) {
            if (it->first == key) {
                bucket.erase(it);
                count--;
                return true;
            }
        }
        return false;
    }

    size_t size() const { return count; }
    size_t bucketCount() const { return table.size(); }

    double loadFactor() const { return (double)count / table.size(); }

    // Number of non-empty buckets (used to demonstrate distribution)
    size_t nonEmptyBuckets() const {
        size_t c = 0;
        for (const auto& bucket : table)
            if (!bucket.empty()) c++;
        return c;
    }

    // Largest bucket size (used to demonstrate worst-case collisions)
    size_t maxBucketSize() const {
        size_t m = 0;
        for (const auto& bucket : table)
            m = max(m, bucket.size());
        return m;
    }

    vector<K> keys() const {
        vector<K> result;
        for (const auto& bucket : table)
            for (const auto& kv : bucket)
                result.push_back(kv.first);
        return result;
    }
};

// A simple open-addressing hash map (linear probing) implemented for strings -> int
// to demonstrate an alternative collision-resolution strategy.
class LinearProbingMap {
private:
    static const int EMPTY = 0;
    static const int OCCUPIED = 1;
    static const int DELETED = 2;

    vector<string> keys_;
    vector<int> values_;
    vector<int> status;
    int count = 0;

    int hashStr(const string& key) const {
        int h = 0;
        for (char c : key) h = (h * 31 + (unsigned char)c) % (int)keys_.size();
        return h;
    }

public:
    explicit LinearProbingMap(int size = 16) : keys_(size), values_(size, 0), status(size, EMPTY) {}

    void put(const string& key, int value) {
        int idx = hashStr(key);
        int start = idx;
        while (status[idx] == OCCUPIED && keys_[idx] != key) {
            idx = (idx + 1) % (int)keys_.size();
            if (idx == start) return; // table full
        }
        if (status[idx] != OCCUPIED) count++;
        keys_[idx] = key;
        values_[idx] = value;
        status[idx] = OCCUPIED;
    }

    optional<int> get(const string& key) const {
        int idx = hashStr(key);
        int start = idx;
        while (status[idx] != EMPTY) {
            if (status[idx] == OCCUPIED && keys_[idx] == key) return values_[idx];
            idx = (idx + 1) % (int)keys_.size();
            if (idx == start) break;
        }
        return nullopt;
    }

    bool remove(const string& key) {
        int idx = hashStr(key);
        int start = idx;
        while (status[idx] != EMPTY) {
            if (status[idx] == OCCUPIED && keys_[idx] == key) {
                status[idx] = DELETED;
                count--;
                return true;
            }
            idx = (idx + 1) % (int)keys_.size();
            if (idx == start) break;
        }
        return false;
    }

    int size() const { return count; }
};

int main() {
    cout << "=== Chained HashMap: Basic Operations ===" << endl;
    HashMap<string, int> map;
    map.put("apple", 5);
    map.put("banana", 3);
    map.put("cherry", 8);

    cout << "apple: "  << *map.get("apple")  << endl;
    cout << "banana: " << *map.get("banana") << endl;
    cout << "contains grape: " << (map.contains("grape") ? "yes" : "no") << endl;

    cout << endl << "=== Updating Existing Keys ===" << endl;
    map.put("apple", 10);
    cout << "apple after update: " << *map.get("apple") << endl;

    cout << endl << "=== Removing Keys ===" << endl;
    cout << "Remove banana: " << (map.remove("banana") ? "success" : "failed") << endl;
    cout << "contains banana after removal: " << (map.contains("banana") ? "yes" : "no") << endl;
    cout << "Remove nonexistent: " << (map.remove("grape") ? "success" : "failed") << endl;

    cout << endl << "=== Dynamic Resizing ===" << endl;
    HashMap<int, string> bigMap(4); // start small to force resizing
    cout << "Initial bucket count: " << bigMap.bucketCount() << endl;
    for (int i = 0; i < 20; i++)
        bigMap.put(i, "value" + to_string(i));
    cout << "After inserting 20 items, bucket count: " << bigMap.bucketCount() << endl;
    cout << "Size: " << bigMap.size() << endl;
    cout << "Load factor: " << bigMap.loadFactor() << endl;
    cout << "Non-empty buckets: " << bigMap.nonEmptyBuckets() << endl;
    cout << "Max bucket size: " << bigMap.maxBucketSize() << endl;

    cout << endl << "=== Verifying All Values After Resize ===" << endl;
    bool allCorrect = true;
    for (int i = 0; i < 20; i++) {
        auto val = bigMap.get(i);
        if (!val || *val != "value" + to_string(i)) {
            allCorrect = false;
            break;
        }
    }
    cout << "All 20 values retrievable and correct: " << (allCorrect ? "yes" : "no") << endl;

    cout << endl << "=== Iterating Keys ===" << endl;
    vector<string> allKeys = map.keys();
    sort(allKeys.begin(), allKeys.end());
    cout << "Keys in HashMap<string,int>: ";
    for (const auto& k : allKeys) cout << k << " ";
    cout << endl;

    cout << endl << "=== Open Addressing (Linear Probing) HashMap ===" << endl;
    LinearProbingMap lpMap(16);
    lpMap.put("one", 1);
    lpMap.put("two", 2);
    lpMap.put("three", 3);
    lpMap.put("four", 4);

    cout << "one: " << *lpMap.get("one") << endl;
    cout << "three: " << *lpMap.get("three") << endl;
    cout << "size: " << lpMap.size() << endl;

    lpMap.remove("two");
    cout << "After removing 'two', contains: " << (lpMap.get("two").has_value() ? "yes" : "no") << endl;
    cout << "size after removal: " << lpMap.size() << endl;

    // Insert enough entries to trigger collisions and probing
    for (int i = 0; i < 10; i++)
        lpMap.put("key" + to_string(i), i * 100);
    cout << "After inserting 10 more keys, size: " << lpMap.size() << endl;
    cout << "key5: " << *lpMap.get("key5") << endl;

    return 0;
}