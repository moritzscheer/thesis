
#pragma once

#include <string>
#include <vector>

int parse_csv_lines(FILE *file, std::vector<std::string> headers);

std::vector<std::string> parse_csv_line(const char *line);
