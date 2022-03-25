#pragma once

#include <chrono>
#include <random>
#include <sstream>
#include <string>
#include <iostream>

#include "List.h"

using std::string, std::stringstream, std::random_device, std::mt19937_64, std::uniform_int_distribution, std::make_shared, std::shared_ptr, std::static_pointer_cast, std::dynamic_pointer_cast;

class Data;
class DataIter;
class Database;

class Data {
private:
    class UID {
    private:
        static mt19937_64 rng;
        static uniform_int_distribution<int> dis;
        static uniform_int_distribution<int> dis2;
        string id;

        static string gen();
    public:
        UID() : id{UID::gen()} {}
        UID(const UID& uid) : id{uid.id} {}

        operator string() const;
        UID& operator =(const UID& uid);
        bool operator ==(const UID& uid) const;
    };

    UID uid;
public:
    Data() = default;

    bool operator ==(const Data& obj) const;
    virtual void write() {
        std::cout << "hi\n";
    }
};


class DataIter {
    friend class Database;
private:
    List<shared_ptr<Data>>::iterator iterator;
public:
    DataIter() = default;
    DataIter(List<shared_ptr<Data>>::iterator iterator) : iterator{iterator} {}

    bool empty() const;
    template <class T>
    shared_ptr<T> ptr() const {
        return dynamic_pointer_cast<T>(*iterator);
    }
    DataIter& operator =(const DataIter& ref) = default;
    bool operator ==(const DataIter& ref) const;
};


class Database {
private:
    List<shared_ptr<Data>> data;
public:
    Database() = default;

    template <class T>
    DataIter add(const shared_ptr<T>& ptr) {
        auto dataPtr = static_pointer_cast<Data>(ptr);
        return data.insert(data.end(), dataPtr);
    }
    void remove(const DataIter& dataIter);
};
