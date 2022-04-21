#include "CSV.h"
#include "Utils.h"

int CSVData::headerSize() const {
    return headers.size();
}

int CSVData::rowSize() const {
    return data.size();
}

List<string> CSVData::getHeaders() const {
    return headers;
}

List<List<string>> CSVData::getData() const {
    return data;
}

void CSVData::setHeaders(const List<string> &list) {
    headers = list;
    data.clear();

    normalizeList(headers);
}

bool CSVData::addRow(const List<string> &list) {
    if (list.size() != headerSize())
        return false;

    data.push_back(list);
    normalizeList(data.back());

    return true;
}

void CSVData::clearData() {
    data.clear();
}

void CSVData::normalizeList(List<string> &list) {
    for(auto &s : list) {
        Utils::toLowerStr(s);
        Utils::trimStr(s);
    }
}



