
#include "utils.hpp"
#include <cstdio>
#include <iostream>
#include <sstream>
#include <unordered_map>
#include <vector>

using namespace std;

int parse_csv_lines(FILE *file, vector<string> headers)
{
    size_t size = ftell(file);

    if (size < 0)
    {
        std::cout << "File is Empty." << std::endl;
        return 0;
    }

    vector<char> buffer(size);
    size_t current = 0;

    while (!fgets(buffer.data(), sizeof(buffer), file))
    {
        auto col = parse_csv_line(current, buffer.data());
    }

    if (feof(file))
    {
        fclose(file);
        return 0;
    }

    return 0;
}

vector<string> parse_csv_data_line(const char *line)
{
    vector<string> out;
    stringstream ss{line};
    string item;

    while (std::getline(ss, item, ','))
    {
        out.push_back(item);
    }

    return out;
}

std::unordered_map<std::string, size_t> parse_csv_header(char *path)
{
    FILE *file = fopen(path, "r");

    if (!file)
    {
        std::cout << -errno;
        return -errno;
    }

    fseek(file, 0, SEEK_END);
    size_t size = ftell(file);

    vector<char> buffer(size);
    if (!fgets(buffer.data(), sizeof(buffer), file))
    {
        throw std::runtime_error("CSV: failed to read header line");
    }

    std::string header = buffer;
    if (!header.empty() && header.back() == '\n')
    {
        header.pop_back();
    }

    std::unordered_map<std::string, size_t> index_map;
    std::stringstream ss(header);
    std::string col;
    size_t idx = 0;

    while (std::getline(ss, col, ','))
    {
        index_map[col] = idx++;
    }

    return index_map;
}
