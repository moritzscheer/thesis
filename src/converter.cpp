
#include "converter.hpp"
#include "utils.hpp"
#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <getopt.h>

int main(int argc, char *argv[])
{
    const char *path    = nullptr;
    const char *columns = nullptr;

    option options[] = {{"file", required_argument, 0, 'f'}, {"columns", required_argument, 0, 'c'}, {0, 0, 0, 0}};

    int opt, i = 0;

    while ((opt = getopt_long(argc, argv, "abc:d:f:", options, &i)) != -1)
    {
        switch (opt)
        {
        case 0:
            printf("option %s", options[i].name);
            if (optarg)
            {
                printf(" with arg %s", optarg);
            }
            printf("\n");
            break;

        case 'f':
        {
            path = optarg;
            break;
        }
        case 'c':
        {
            columns = optarg;
            break;
        }
        case '?':
            break;

        default:
            abort();
        }
    }

    if (!path)
    {
        printf("%s [-s/--source path to source file]", argv[0]);
        return EXIT_FAILURE;
    }

    if (!columns)
    {
        printf("%s [-c/--columns %s] Corresponding names in source File", argv[0], schema);
        return EXIT_FAILURE;
    }

    FILE *file = fopen(path, "r");

    if (!file)
    {
        printf("Unable to open File %s: %s", path, strerror(errno));
        return EXIT_FAILURE;
    }

    size_t size = ftell(file);

    if (size < 0)
    {
        printf("File %s is empty.", path);
        return EXIT_FAILURE;
    }

    std::vector<char> buffer(size);

    if (!fgets(buffer.data(), sizeof(buffer), file))
    {
        auto col = parse_csv_line(buffer.data());
    }

    fclose(file);

    return EXIT_SUCCESS;
}
