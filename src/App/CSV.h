#pragma once

#include <string>
#include <stdexcept>
#include <sstream>

#include "List.h"
#include "../Constants.h"

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
    };

    class Parser {
    protected:
        void parseHeader();
        void parseContent();

    private:
        string _file;
        const char _sep;
        List<string> _originalFile;
        List<string> _header;
        List<Row *> _content;

    public:
        Parser(const string &, char sep = CSV::SEPARATOR);
        ~Parser();

        Row &getRow(int row) const;
        int rowCount() const;
        int columnCount() const;
        List<string> getHeader() const;

        Row &operator[](int row) const;
    };

    class CSVWriter {
    private:
        string separator;
        int columnNum;
        int valueCount;
        bool firstRow;
        stringstream ss;
        
    public:
        CSVWriter();
        ~CSVWriter();

        CSVWriter& add(string str);
        template<typename T>
        CSVWriter& add(T str) {
            if (this->columnNum > -1) {
                //if autoNewRow is enabled, check if we need a line break
                if (this->valueCount == this->columnNum) {
                    this->newRow();
                }
            }
            if(valueCount > 0)
                this->ss << this->separator;
            this->ss << str;
            this->valueCount++;

            return *this;
        }
        string toString();
        CSVWriter& newRow();
        bool writeToFile(const string &filename, bool append = false);
        //you can use this reset method in destructor if you're in heap mem.
        void resetContent();

        template<typename T>
        CSVWriter& operator<<(const T &t) {return this->add(t);}
        friend std::ostream& operator<<(std::ostream &os, CSVWriter &csv) {
            return os << csv.toString();
        }
    };

    CSVData tryParse(const string &file_path);
}