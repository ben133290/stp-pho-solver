//
// Created by Benedikt Heuser on 16.04.24.
//
#define OS_MAC

#include "PDB-generator.h"

int main(int argc, char *argv[]) {
    CLI::App app;
    initCLI(argv, app);
    CLI11_PARSE(app, argc, argv);

    std::cout << "Attempting to build PDB" << std::endl;
    BuildSTPPDB<GRID_SIZE, GRID_SIZE>();

    std::cout << "Memory used: " << utils::get_peak_memory_in_kb() << " kb" << std::endl;
    return 0;
}

// Sets command line options
void initCLI(char **&argv, CLI::App &app) {
    argv = app.ensure_utf8(argv);
    app.add_option("--pattern", pattern, "Specify a pattern for pdb");
    app.add_option("-p", pattern, "Specify a pattern for pdb");
    app.add_option("--path", pdbPath, "Specify the path where the file should be saved");
}
