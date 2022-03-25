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

bool DataIter::empty() const {return iterator == nullptr;}

bool DataIter::operator==(const DataIter &ref) const {return this->iterator == ref.iterator;}

void Database::remove(const DataIter &dataRef) {
    data.remove(dataRef.iterator);
}
