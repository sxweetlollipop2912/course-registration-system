#include <fstream>
#include <iomanip>
#include <iostream>

#include "CSV.h"
#include "Utils.h"

using std::ios, std::ifstream, std::endl, std::cerr, std::ios_base;

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

    Parser::Parser(const string &data, char sep)
            : _sep(sep) {
        string line;

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

    Parser::~Parser() {
        for (auto &it : _content)
            delete it;
    }

    void Parser::parseHeader() {
        stringstream ss(_originalFile[0]);
        string item;

        while (getline(ss, item, _sep))
            _header.push_back(item);
    }

    void Parser::parseContent() {
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
                
                else if (it->at(i) == _sep && !quoted) {
                    auto s = it->substr(tokenStart, i - tokenStart);
                    while (s.size() > 1 &&s[0] == '"' &&s.back() == '"') {
                        s.erase(0, 1);
                        s.pop_back();
                    }

                    row->push(s);
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

    Row::Row(const List<string> &header) : _header(header) {}

    int Row::size() const {
        return _values.size();
    }

    void Row::push(const string &value) {
        _values.push_back(value);
    }

    string Row::operator[](int valuePosition) const {
        if (valuePosition < _values.size())
            return _values[valuePosition];

        throw Error("can't return this value (doesn't exist)");
    }

    string Row::operator[](const string &key) const {
        int pos = 0;
        for (const auto &it : _header) {
            if (key == it)
                return _values[pos];
            pos++;
        }

        throw Error("can't return this value (doesn't exist)");
    }

    CSVWriter::CSVWriter() {
        this->firstRow = true;
        this->separator = CSV::SEPARATOR;
        this->columnNum = -1;
        this->valueCount = 0;
    }

    CSVWriter& CSVWriter::add(string str) {
        //if " character was found, escape it
        size_t position = str.find('\"', 0);
        bool foundQuotationMarks = position != string::npos;
        while(position != string::npos) {
            str.insert(position, "\"");
            position = str.find('\"', position + 2);
        }
        if(foundQuotationMarks) {
            str = "\"" + str + "\"";
        }
        else if (str.find(this->separator) != string::npos) {
            //if separator was found and string was not escaped before, surround string with "
            str = "\"" + str + "\"";
        }
        return this->add<string>(str);
    }

    string CSVWriter::toString() {return ss.str();}

    CSVWriter& CSVWriter::newRow() {
        if (!this->firstRow || this->columnNum > -1) {
            ss << endl;
        }
        else {
            //if the row is the first row, do not insert a new line
            this->firstRow = false;
        }
        valueCount = 0;
        return *this;
    }

    bool CSVWriter::writeToFile(const string &filename, bool append) {
        ofstream file;
        bool appendNewLine = false;
        if (append) {
            //check if last char of the file is newline
            ifstream fin;
            fin.open(filename);
            if (fin.is_open()) {
                fin.seekg(-1, ios_base::end); //go to end of file
                int lastChar = fin.peek();
                if (lastChar != -1 && lastChar != '\n') //if file is not empty and last char is not new line char
                    appendNewLine = true;
            }
            file.open(filename.c_str(), ios::out | ios::app);
        }
        else {
            file.open(filename.c_str(), ios::out | ios::trunc);
        }
        if(!file.is_open())
            return false;

        if(append && appendNewLine)
            file << endl;
        file << this->toString();
        file.close();

        return file.good();
    }

    void CSVWriter::resetContent() {
        const static stringstream initial;

        ss.str(string());
        ss.clear();
        ss.copyfmt(initial);
    }

    CSVWriter::~CSVWriter() {
        resetContent();
    }
}



