#include <utility>
#include <string>

#include "Database.h"

using std::ofstream, std::ifstream;

string Data::UID::gen() {
    std::mt19937_64 mt(std::chrono::system_clock::now().time_since_epoch() / std::chrono::microseconds(1));
    return std::to_string(std::uniform_int_distribution<long long>(1e17, 1e18 - 1)(mt));
}

Data::UID::operator string() const { return id; }

Data::UID &Data::UID::operator=(const Data::UID &uid) {
    this->id = uid;
    return *this;
}

bool Data::UID::operator==(const Data::UID &uid) const { return (string) (*this) == (string) uid; }

bool Data::operator==(const Data &obj) const { return this->uid == obj.uid; }

bool DataIter::empty() const { return !(bool) iterator || iterator == nullptr; }

bool DataIter::operator==(const DataIter &ref) const { return this->iterator == ref.iterator; }

DataIter::operator bool() const {
    return !this->empty();
}

Data::UID DataIter::uid() const {
    return !iterator ? tmp_uid : (*iterator)->uid;
}

void DataIter::clear() {
    data_ptr = nullptr;
    iterator = nullptr;
    tmp_uid = Data::UID();
}

void Database::remove(const DataIter &data_iter) {
    if (data_iter)
        data.remove(data_iter.iterator);
}

void Database::remove(const List<DataIter> &data_iters) {
    for (const auto &data_iter: data_iters)
        if (data_iter)
            remove(data_iter);
}

DataIter Database::get(const Data::UID &uid) {
    auto it = data.find_if([&](const shared_ptr<Data> &ptr) {
        return ptr->uid == uid;
    });

    if (it != data.end()) return it;

    return {};
}

DataIter Database::get(std::function<bool(const shared_ptr<Data> &)> func) {
    auto it = data.find_if(std::move(func));

    if (it != data.end()) return it;

    return {};
}

List<DataIter> Database::getAll(const std::function<bool(const shared_ptr<Data> &)> &func) {
    List<DataIter> list;
    for (auto it = data.begin(); it != data.end(); ++it) {
        if (func(*it)) list.push_back(it);
    }

    return list;
}

void Database::clean() {
    for (auto it = data.begin(); it != data.end();) {
        /// Remove if data is not used anywhere.
        if ((*it).use_count() == 1)
            it = data.remove(it);
        else ++it;
    }
}

int Database::size() const {
    return data.size();
}

void Data::load(Database &database) {}
