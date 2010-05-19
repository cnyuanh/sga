//-----------------------------------------------
// Copyright 2010 Wellcome Trust Sanger Institute
// Written by Jared Simpson (js18@sanger.ac.uk)
// Released under the GPL
//-----------------------------------------------
//
// merge - merge read files and their indices
//
#include <iostream>
#include <fstream>
#include "SGACommon.h"
#include "Util.h"
#include "merge.h"
#include "SACAInducedCopying.h"
#include "BWTDiskConstruction.h"
#include "BWT.h"

//
// Getopt
//
#define SUBPROGRAM "merge"

static const char *MERGE_VERSION_MESSAGE =
SUBPROGRAM " Version " PACKAGE_VERSION "\n"
"Written by Jared Simpson.\n"
"\n"
"Copyright 2010 Wellcome Trust Sanger Institute\n";

static const char *MERGE_USAGE_MESSAGE =
"Usage: " PACKAGE_NAME " " SUBPROGRAM " [OPTION] ... READS1 READS2 ...\n"
"Merge the sequence files READS1, READS2, ... READSN and their associated indices into a single file/index\n"
"\n"
"  -v, --verbose                        display verbose output\n"
"      --help                           display this help and exit\n"
"  -p, --prefix=PREFIX                  write final index to file using PREFIX (the default is to concatenate the input filenames)\n"
"\nReport bugs to " PACKAGE_BUGREPORT "\n\n";

namespace opt
{
    static unsigned int verbose;
    static std::string prefix;
}

static const char* shortopts = "p:m:v";

enum { OPT_HELP = 1, OPT_VERSION };

static const struct option longopts[] = {
    { "verbose",     no_argument,       NULL, 'v' },
    { "prefix",      required_argument, NULL, 'p' },
    { "help",        no_argument,       NULL, OPT_HELP },
    { "version",     no_argument,       NULL, OPT_VERSION },
    { NULL, 0, NULL, 0 }
};

int mergeMain(int argc, char** argv)
{
    parseMergeOptions(argc, argv);
    StringVector inFiles;
    while(optind < argc)
    {
        inFiles.push_back(argv[optind++]);
    }
    assert(inFiles.size() == 2);
    mergeIndependentIndices(inFiles[0], inFiles[1], BWT_EXT, SAI_EXT, false);
    mergeIndependentIndices(inFiles[0], inFiles[1], RBWT_EXT, RSAI_EXT, true);

    return 0;
}

// 
// Handle command line arguments
//
void parseMergeOptions(int argc, char** argv)
{
    bool die = false;
    for (char c; (c = getopt_long(argc, argv, shortopts, longopts, NULL)) != -1;) 
    {
        std::istringstream arg(optarg != NULL ? optarg : "");
        switch (c) 
        {
            case 'p': arg >> opt::prefix; break;
            case '?': die = true; break;
            case 'v': opt::verbose++; break;
            case OPT_HELP:
                std::cout << MERGE_USAGE_MESSAGE;
                exit(EXIT_SUCCESS);
            case OPT_VERSION:
                std::cout << MERGE_VERSION_MESSAGE;
                exit(EXIT_SUCCESS);
        }
    }

    if (argc - optind < 1) 
    {
        std::cerr << SUBPROGRAM ": missing arguments\n";
        die = true;
    } 

    if (die) 
    {
        std::cerr << "Try `" << SUBPROGRAM << " --help' for more information.\n";
        exit(EXIT_FAILURE);
    }
}