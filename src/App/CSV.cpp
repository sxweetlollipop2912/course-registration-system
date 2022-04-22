#include <fstream>
#include <iomanip>
#include <iostream>

#include "CSV.h"
#include "Utils.h"

using std::ios, std::ifstream, std::istringstream, std::endl, std::cerr;

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
    data.back().for_each([](string &s) {
        Utils::trimStr(s);
    });

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

namespace CSVIO {
    CSVData tryParse(const string &file_path) {
        CSVData data;

        try {
            Parser file = Parser(file_path);

            data.setHeaders(file.getHeader());
            for(int i = 0; i < file.rowCount(); i++) {
                List<string> row;
                for(int j = 0; j < file.columnCount(); j++) {
                    row.push_back(file[i][j]);
                }
                data.addRow(row);
            }
        }
        catch (Error &e) {
            cerr << e.what() << std::endl;
        }

        return data;
    }

    Parser::Parser(const string &data, const DataType &type, char sep)
            : _type(type), _sep(sep) {
        string line;
        if (type == eFILE) {
            _file = data;
            ifstream ifile(_file.c_str());
            
            if (ifile.is_open()) {
                while (ifile.good()) {
                    getline(ifile, line);
                    if (!line.empty())
                        _originalFile.push_back(line);
                }
                ifile.close();

                if (_originalFile.empty())
                    throw Error(string("No Data in ").append(_file));

                parseHeader();
                parseContent();
            }
            else
                throw Error(string("Failed to open ").append(_file));
        }
        else {
            istringstream stream(data);
            while (getline(stream, line))
                if (!line.empty())
                    _originalFile.push_back(line);
            if (_originalFile.empty())
                throw Error(string("No Data in pure content"));

            parseHeader();
            parseContent();
        }
    }

    Parser::~Parser() {
        for (auto & it : _content)
            delete it;
    }

    void Parser::parseHeader() {
        stringstream ss(_originalFile[0]);
        string item;

        while (getline(ss, item, _sep))
            _header.push_back(item);
    }

    void Parser::parseContent()
    {
        auto it = _originalFile.begin();
        it++; // skip header

        for (; it != _originalFile.end(); it++) {
            bool quoted = false;
            int tokenStart = 0;
            int i = 0;

            Row *row = new Row(_header);

            for (; i != it->length(); i++) {
                if (it->at(i) == '"')
                    quoted = !(quoted);
                
                else if (it->at(i) == ',' && !quoted) {
                    row->push(it->substr(tokenStart, i - tokenStart));
                    tokenStart = i + 1;
                }
            }

            //end
            row->push(it->substr(tokenStart, it->length() - tokenStart));

            // if value(s) missing
            if (row->size() != _header.size())
                throw Error("corrupted data !");
            
            _content.push_back(row);
        }
    }

    Row &Parser::getRow(int rowPosition) const {
        if (rowPosition < _content.size())
            return *(_content[rowPosition]);
        
        throw Error("can't return this row (doesn't exist)");
    }

    Row &Parser::operator[](int rowPosition) const {
        return Parser::getRow(rowPosition);
    }

    int Parser::rowCount() const {
        return _content.size();
    }

    int Parser::columnCount() const {
        return _header.size();
    }

    List<string> Parser::getHeader() const {
        return _header;
    }

    string Parser::getHeaderElement(int pos) const {
        if (pos >= _header.size())
            throw Error("can't return this header (doesn't exist)");

        return _header[pos];
    }

    bool Parser::deleteRow(int pos) {
        if (pos < _content.size()) {
            auto it = _content.begin() + pos;
            delete *it;
            _content.remove(it);

            return true;
        }
        return false;
    }

    bool Parser::addRow(int pos, const List<string> &r) {
        Row *row = new Row(_header);

        for (const auto & it : r)
            row->push(it);

        if (pos <= _content.size()) {
            _content.insert(_content.begin() + pos, row);

            return true;
        }

        return false;
    }

    void Parser::sync() const {
        if (_type == DataType::eFILE) {
            ofstream f;
            f.open(_file, ios::out | ios::trunc);

            // header
            int i = 0;
            for (auto it = _header.begin(); it != _header.end(); it++, i++) {
                f << *it;
                if (i < _header.size() - 1)
                    f << ",";
                else
                    f << endl;
            }

            for (auto it : _content)
                f << *it << endl;

            f.close();
        }
    }

    string Parser::getFileName() const {
        return _file;
    }

    Row::Row(const List<string> &header) : _header(header) {}

    int Row::size() const {
        return _values.size();
    }

    void Row::push(const string &value) {
        _values.push_back(value);
    }

    bool Row::set(const string &key, const string &value) {
        int pos = 0;
        for (const auto & it : _header) {
            if (key == it) {
                _values[pos] = value;

                return true;
            }
            pos++;
        }

        return false;
    }

    string Row::operator[](int valuePosition) const {
        if (valuePosition < _values.size())
            return _values[valuePosition];

        throw Error("can't return this value (doesn't exist)");
    }

    string Row::operator[](const string &key) const {
        int pos = 0;
        for (const auto & it : _header) {
            if (key == it)
                return _values[pos];
            pos++;
        }

        throw Error("can't return this value (doesn't exist)");
    }

    ostream &operator<<(ostream &os, const Row &row) {
        for (const auto & _value : row._values)
            os << _value << " | ";

        return os;
    }

    ofstream &operator<<(ofstream &os, const Row &row) {
        int i = 0;
        for(const auto & _value : row._values) {
            os << _value;
            if (i < row._values.size() - 1)
                os << ",";
            ++i;
        }

        return os;
    }
}



