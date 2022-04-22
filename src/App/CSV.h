#pragma once

#include <string>
#include <stdexcept>
#include <sstream>

#include "List.h"

using std::string, std::stringstream, std::ostream, std::ofstream, std::runtime_error;

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

namespace CSVIO {
    class Error : public runtime_error {
    public:
        Error(const string &msg):
                runtime_error(string("CSVparser : ").append(msg)) {}
    };

    class Row {
    private:
        const List<string> _header;
        List<string> _values;

    public:
        Row(const List<string> &);

        int size() const;
        void push(const string &);
        bool set(const string &, const string &);

        template<typename T>
        const T getValue(int pos) const {
            if (pos < _values.size()) {
                T res;
                stringstream ss;
                ss << _values[pos];
                ss >> res;
                return res;
            }
            throw Error("can't return this value (doesn't exist)");
        }
        string operator[](int) const;
        string operator[](const string &valueName) const;
        friend ostream& operator<<(ostream& os, const Row &row);
        friend ofstream& operator<<(ofstream& os, const Row &row);
    };

    enum DataType {
        eFILE = 0,
        ePURE = 1
    };

    class Parser {
    protected:
        void parseHeader();
        void parseContent();

    private:
        string _file;
        const DataType _type;
        const char _sep;
        List<string> _originalFile;
        List<string> _header;
        List<Row *> _content;

    public:
        Parser(const string &, const DataType &type = eFILE, char sep = ',');
        ~Parser();

        Row &getRow(int row) const;
        int rowCount() const;
        int columnCount() const;
        List<string> getHeader() const;
        string getHeaderElement(int pos) const;
        string getFileName() const;

        bool deleteRow(int row);
        bool addRow(int pos, const List<string> &);
        void sync() const;

        Row &operator[](int row) const;
    };

    CSVData tryParse(const string &file_path);
    bool write(const CSVData &data, const string &file_path);
}