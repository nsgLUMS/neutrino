/*
    Consistent CircularMap class to provide concurrent safe
    access to a hash table using mutexes.
*/

#ifndef __map_h__
#define __map_h__

#include <map>
#include <iostream>
#include <mutex>

using namespace std;

class Map { };

template <typename K, typename V>
class CircularMap : public Map
{
private:
    map<K, V> ring;
    pthread_mutex_t m_mutex;
    pthread_cond_t m_condv;

public:
    CircularMap()
    {
        pthread_mutex_init(&m_mutex, NULL);
        pthread_cond_init(&m_condv, NULL);
    }

    ~CircularMap()
    {
        pthread_mutex_destroy(&m_mutex);
        pthread_cond_destroy(&m_condv);
    }

    void lock()
    {
        pthread_mutex_lock(&m_mutex);
    }

    void unlock()
    {
        pthread_mutex_unlock(&m_mutex);
    }

    void insert(K key, V value)
    {
        ring[key] = value;
    }

    void remove(K key)
    {
        pthread_mutex_lock(&m_mutex);
        while (ring.size() == 0)
            pthread_cond_wait(&m_condv, &m_mutex);
        ring.erase(key);
        pthread_mutex_unlock(&m_mutex);
    }

    map<K, V> &content()
    {
        pthread_mutex_lock(&m_mutex);
        map<K, V> &to_return = ring;
        pthread_mutex_unlock(&m_mutex);
        return to_return;
    }

    V get(K key)
    {
        V value = ring[key];
        return value;
    }

    V upper(K key)
    {
        pthread_mutex_lock(&m_mutex);
        V value;
        auto it = ring.upper_bound(key);
        value = it->second;

        if (it == ring.end())
            value = ring.begin()->second;
        pthread_mutex_unlock(&m_mutex);
        return value;
    }

    V lower(K key)
    {
        pthread_mutex_lock(&m_mutex);
        V value;
        auto it = ring.lower_bound(key);

        if (it == ring.begin())
        {
            value = (ring.rbegin())->second;
        }
        else
        {
            it--;
            if (it == ring.end())
            {
                it--;
                value = ring.begin()->second;
            }
            else
            {
                value = it->second;
            }
        }
        pthread_mutex_unlock(&m_mutex);
        return value;
    }

    bool isEmpty()
    {
        pthread_mutex_lock(&m_mutex);
        int size = ring.size();
        pthread_mutex_unlock(&m_mutex);
        return size == 0;
    }

    int size()
    {
        pthread_mutex_lock(&m_mutex);
        int size = ring.size();
        pthread_mutex_unlock(&m_mutex);
        return size;
    }

    bool has(K key)
    {
        bool result = false;
        pthread_mutex_lock(&m_mutex);
        if (ring.find(key) != ring.end())
            result = true;
        pthread_mutex_unlock(&m_mutex);
        return result;
    }

    void display()
    {
        pthread_mutex_lock(&m_mutex);
        for (auto const &pair : ring)
            cout << "{" << pair.first << ": " << pair.second << "}\n";
        pthread_mutex_unlock(&m_mutex);
    }

    string toString() const
    {
        pthread_mutex_lock(&m_mutex);
        string buffer("");
        for (auto const &pair : ring)
            buffer += "{" + pair.first + ": " + pair.second + "}";
        pthread_mutex_unlock(&m_mutex);
        return buffer;
    }

    bool count(K key)
    {
        bool count = false;
        count = ring.count(key);
        return count;
    }

    typename map<K, V>::iterator upper_iterator(K key)
    {
        typename map<K, V>::iterator it;
        pthread_mutex_lock(&m_mutex);
        it = ring.upper_bound(key);
        pthread_mutex_unlock(&m_mutex);
        return it;
    }

    typename map<K, V>::iterator lower_iterator(K key)
    {
        typename map<K, V>::iterator it;
        pthread_mutex_lock(&m_mutex);
        it = ring.lower_bound(key);
        pthread_mutex_unlock(&m_mutex);
        return it;
    }
};

#endif