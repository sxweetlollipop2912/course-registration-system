#pragma once

#include <chrono>
#include <random>
#include <sstream>
#include <string>
#include <iostream>

#include "List.h"
#include "../Enums.h"
#include "Utils.h"

using std::string, std::stringstream, std::random_device, std::mt19937_64, std::uniform_int_distribution, std::make_shared, std::shared_ptr, std::static_pointer_cast, std::dynamic_pointer_cast;

class Data;
class DataIter;
class Database;

class Data {
public:
    class UID {
    private:
        static mt19937_64 rng;
        static uniform_int_distribution<int> dis;
        static uniform_int_distribution<int> dis2;
    public:
        string id;

        UID() : id{UID::gen()} {}
        UID(const UID& uid) : id{uid.id} {}

        static string gen();

        operator string() const;
        UID& operator =(const UID& uid);
        bool operator ==(const UID& uid) const;
    };

    UID uid;
    DataType data_type;

    Data() : data_type{DataType::Other} {}

    virtual void load(Database &database);

    bool operator ==(const Data& obj) const;
    friend std::ostream& operator<<(std::ostream &os, const Data &data) {
        os << data.uid.id << '\n';
        os << (int) data.data_type << '\n';

        return os;
    }
    friend std::istream& operator>>(std::istream &is, Data &data) {
        Utils::getline(is, data.uid.id);
        int type; is >> type;
        data.data_type = static_cast<DataType>(type);

        return is;
    }
};


class DataIter {
    friend class Database;
private:
    /// For smart pointer counting purposes.
    /// This is quite dumb tbh.
    shared_ptr<Data> data_ptr;
    List<shared_ptr<Data>>::iterator iterator;
    Data::UID tmp_uid;
public:
    DataIter() = default;
    DataIter(List<shared_ptr<Data>>::iterator iterator) : data_ptr{nullptr}, iterator{iterator} {
        if ((bool)iterator) data_ptr = *iterator;
    }

    bool empty() const;
    Data::UID uid() const;
    template <class T>
    shared_ptr<T> ptr() const {
        if ((bool)iterator)
            return dynamic_pointer_cast<T>(*iterator);
        return shared_ptr<T>(nullptr);
    }
    DataIter& operator =(const DataIter& iter) = default;
    bool operator ==(const DataIter& iter) const;
    operator bool() const;

    template <class T>
    operator shared_ptr<T>() {
        if ((bool)iterator)
            return dynamic_pointer_cast<T>(*iterator);
        return shared_ptr<T>(nullptr);
    }

    friend std::ostream& operator<<(std::ostream &os, const DataIter &obj) {
        os << obj.uid().id;

        return os;
    }
    friend std::istream& operator>>(std::istream &is, DataIter &obj) {
        Data::UID uid;
        Utils::getline(is, uid.id);
        obj.tmp_uid = uid;

        return is;
    }
};


class Database {
    friend class App;
private:
    List<shared_ptr<Data>> data;
public:
    Database() = default;

    int size() const;

    template <class T>
    DataIter add(const shared_ptr<T>& ptr) {
        auto dataPtr = static_pointer_cast<Data>(ptr);
        return data.insert(data.end(), dataPtr);
    }
    /// Note: This method can't check if `data_iter` is an element of database.
    void remove(const DataIter &data_iter);
    /// Note: This method can't check if any of `data_iters` is an element of database.
    void remove(const List<DataIter> &data_iters);
    /// Returns empty DataIter if no data by this UID is found.
    DataIter get(const Data::UID &uid);
    /// Returns empty DataIter if no data satisfying `func` is found.
    DataIter get(std::function<bool(const shared_ptr<Data>&)> func);
    /// Returns empty DataIter if no data satisfying `func` is found.
    List<DataIter> getAll(const std::function<bool(const shared_ptr<Data>&)>& func);
    /// Removes unused data from database. Doesn't work in case of cyclic dependencies.
    void clean();

    friend std::ostream& operator<<(std::ostream &os, const Database &obj) {
        os << obj.data.size() << '\n';

        for(const auto &e : obj.data) {
            os << e->uid.id << '\n';
            os << (int)e->data_type << '\n';
        }

        return os;
    }

    friend std::istream& operator>>(std::istream &is, Database &obj) {
        int sz; is >> sz;
        obj.data.resize(sz);

        for(auto& e : obj.data) {
            e = make_shared<Data>();
            Utils::getline(is, e->uid.id);
            int type; is >> type;
            e->data_type = static_cast<DataType>(type);
        }

        return is;
    }
};
