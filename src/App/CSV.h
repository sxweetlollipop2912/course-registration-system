#pragma once

#include <string>

#include "List.h"

using std::string;

class CSVData {
    List<string> headers;
    List<List<string>> data;

    /// Sets headers to lowercase.
    static void normalizeList(List <string> &list);
public:
    CSVData() = default;

    int headerSize() const;
    int rowSize() const;
    List<string> getHeaders() const;
    List<List<string>> getData() const;
    void clearData();
    void setHeaders(const List<string> &list);
    /// True if succeeded, or false if:\n
    /// > Number of columns does not match number of headers (headerSize()).
    bool addRow(const List<string> &list);
};

namespace CSV {
    CSVData tryParse(const string &file_path);
    bool write(const CSVData &data, const string &file_path);
}