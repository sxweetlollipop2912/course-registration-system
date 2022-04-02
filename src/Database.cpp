#include "Database.h"

auto Data::UID::rng = mt19937_64(random_device()());
auto Data::UID::dis = uniform_int_distribution<int>(0, 15);
auto Data::UID::dis2 = uniform_int_distribution<int>(8, 11);

string Data::UID::gen() {
    stringstream ss;
    int i;
    ss << std::hex;
    for (i = 0; i < 8; i++) ss << dis(rng);
    ss << "-";
    for (i = 0; i < 4; i++) ss << dis(rng);
    ss << "-4";
    for (i = 0; i < 3; i++) ss << dis(rng);
    ss << "-";
    ss << dis2(rng);
    for (i = 0; i < 3; i++) ss << dis(rng);
    ss << "-";
    for (i = 0; i < 12; i++) ss << dis(rng);
    return ss.str();
}

Data::UID::operator string() const {return id;}
Data::UID &Data::UID::operator=(const Data::UID &uid) { this->id = uid; return *this;}
bool Data::UID::operator==(const Data::UID &uid) const {return (string)(*this) == (string)uid;}

bool Data::operator==(const Data &obj) const {return this->uid == obj.uid;}

bool DataIter::empty() const {return !(bool)iterator || iterator == nullptr;}

bool DataIter::operator==(const DataIter &ref) const {return this->iterator == ref.iterator;}

DataIter::operator bool() const {
    return !this->empty();
}

Data::UID DataIter::uid() const {
    return (*iterator)->uid;
}

void Database::remove(const DataIter &data_iter) {
    data.remove(data_iter.iterator);
}

DataIter Database::getByUID(const Data::UID &uid) {
    auto it = data.find_if([&](const shared_ptr<Data> &ptr) {
        return ptr->uid == uid;
    });

    if (it != data.end()) return it;

    return {};
}

DataIter Database::get(std::function<bool(const shared_ptr<Data> &)> func) {
    auto it = data.find_if(func);

    if (it != data.end()) return it;

    return {};
}

void Database::clean() {
    for(auto it = data.begin(); it != data.end();) {
        /// Remove if data is not used anywhere.
        if ((*it).use_count() == 1)
            it = data.remove(it);
        else ++it;
    }
}

int Database::size() const {
    return data.size();
}
