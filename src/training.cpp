
#include <cerrno>
#include <exception>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#include "training.hpp"

using namespace std;

vector<RawData> read_measurements(string path)
{
    const string ext = ".csv";

    if (path.compare(path.size() - ext.size(), ext.size(), ext) != 0)
    {
        printf("File \"%s\" is not an CSV File.\n", path.c_str());
        throw exception();
    }

    FILE *file = fopen(path.c_str(), "r");

    if (!file)
    {
        printf("Unable to open file \"%s\": %s\n", path.c_str(), strerror(errno));
        throw exception();
    }

    if (fseek(file, 0, SEEK_END) != 0)
    {
        printf("Seek failed: %s\n", strerror(errno));
        fclose(file);
        throw exception();
    }

    long size = ftell(file);

    if (size <= 0)
    {
        printf("File \"%s\" is empty.\n", path.c_str());
        fclose(file);
        throw exception();
    }

    rewind(file);

    char line[size];
    int res    = 0;
    int header = 0;

    vector<RawData> dest;

    while (fgets(line, size, file))
    {
        res = parse_csv_line(dest, line, header);

        if (res != 0)
        {
            throw exception();
        }
    }

    fclose(file);

    return dest;
}

int parse_csv_line(csv_file *dest, char *line, int &header)
{
    if (header)
    {
        int res;

        res = parse_csv_header(line, "id");

        if (res < 0)
        {
            return res;
        }

        res = parse_csv_header(line, "soc");

        if (res < 0)
        {
            return res;
        }

        res = parse_csv_header(line, "timestemp");

        if (res < 0)
        {
            return res;
        }

        res = parse_csv_header(line, "power");

        if (res < 0)
        {
            return res;
        }

        header = !header;

        return 0;
    }

    return parse_csv_data(dest, line);
}

int parse_csv_header(char *line, string target)
{
    char input[4096];

    while (true)
    {
        printf("\rSpecifiy column name for %s: ", target.data());

        ssize_t nread = read(0, input, 4096);

        if (nread < 0)
        {
            return -errno;
        }

        if (nread == 0)
        {
            continue;
        }

        input[nread] = '\0';

        char *newline = strchr(input, '\n');

        if (newline)
        {
            *newline = '\0';
        }

        size_t input_len = strlen(input);

        int index         = 0;
        const char *start = line;

        while (*start)
        {
            const char *end = strchr(start, ';');
            size_t len      = end ? (size_t)(end - start) : strlen(start);

            if (nread == len && strncmp(start, input, len) == 0)
            {
                return index;
            }

            if (!end)
            {
                break;
            }

            start = end + 1;
            index++;
        }

        return -1;
    };
}

int parse_csv_data(csv_file *dest, const char *line)
{
    const char *start = line;
    int index         = 0;

    RawData data = {0};
    bool got_uid = false;
    bool got_soc = false;
    bool got_ts  = false;
    bool got_pwr = false;

    while (*start)
    {
        const char *end = strchr(start, ';');
        size_t len      = end ? (size_t)(end - start) : strlen(start);

        char token[256];
        if (len >= sizeof(token))
            return -1;
        memcpy(token, start, len);
        token[len] = '\0';

        char *conv_end = NULL;

        if (index == dest->id)
        {
            long v = strtol(token, &conv_end, 10);
            if (conv_end == token)
                return -1;
            data.uid = v;
            got_uid  = true;
        }
        else if (index == dest->soc)
        {
            double v = strtod(token, &conv_end);
            if (conv_end == token)
                return -1;
            data.soc = v;
            got_soc  = true;
        }
        else if (index == dest->timestemp)
        {
            unsigned long v = strtoul(token, &conv_end, 10);
            if (conv_end == token)
                return -1;
            data.timestemp = v;
            got_ts         = true;
        }
        else if (index == dest->power)
        {
            double v = strtod(token, &conv_end);
            if (conv_end == token)
                return -1;
            data.power = v;
            got_pwr    = true;
        }

        if (!end)
            break;

        start = end + 1;
        index++;
    }

    if (got_uid && got_soc && got_ts && got_pwr)
    {
        dest->data.push_back(data);
        return 0;
    }

    return -1;
}

int validate_csv_write_file(vector<RawData> *source, const char *path)
{
    FILE *file = fopen(path, "a");
    if (!file)
    {
        printf("Unable to open file \"%s\": %s\n", path, strerror(errno));
        return errno;
    }

    long size = ftell(file);

    if (size > 0)
    {
        printf("File \"%s\" is not empty.\n", path);
        fclose(file);
        return errno;
    }

    for (RawData row : *source)
    {
    }

    fclose(file);
}

int write_csv_line(RawData *line, bool &header)
{
    char input[4096];

    printf("Path to CSV File: ");

    while (true)
    {
        ssize_t nread = read(0, input, 4096);

        if (nread < 0)
        {
            return -errno;
        }

        if (nread == 0)
        {
            continue;
        }

        input[nread] = '\0';

        int res = read_measurements(dest, input);

        if (res != 0)
        {
            return res;
        }
    };
}
