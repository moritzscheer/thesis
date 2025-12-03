
#pragma once

#include <string>
#include <vector>

inline const char *schema = "uid,timestemp,soc,txp";

struct RawData
{
    int uid;
    double soc;
    double power;
    int64_t timestemp;
};

struct csv_file
{
    int id;
    int soc;
    int power;
    int timestemp;

    csv_file *operator=(csv_file *base);
};

std::vector<RawData> generate_measurements();

std::vector<RawData> read_measurements(std::string path);

int validate_read_csv_file(csv_file *dest, const char *path);

int parse_csv_line(csv_file *dest, char *line, int &header);

int parse_csv_header(char *line, std::string target);

int parse_csv_data(csv_file *dest, char *line);

int write_csv_file(csv_file *dest, const char *path);
