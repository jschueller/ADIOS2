#ifndef _WIN32
#include "strings.h"
#else
#define strcasecmp _stricmp
#endif
std::string fname = "ADIOS2Common";
std::string engine = "sst";
adios2::Params engineParams = {}; // parsed from command line

bool SharedIO = false;
bool SharedVar = false;
int DurationSeconds = 60 * 60 * 24 * 365; // one year default
int DelayMS = 1000;                       // one step per sec default
int CompressSz = 0;
int CompressZfp = 0;
int TimeGapExpected = 0;
int IgnoreTimeGap = 1;
int ExpectOpenTimeout = 0;
int ZeroDataVar = 0;
int ZeroDataRank = 0;
std::string shutdown_name = "DieTest";
adios2::Mode GlobalWriteMode = adios2::Mode::Deferred;

static std::string Trim(std::string &str)
{
    size_t first = str.find_first_not_of(' ');
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last - first + 1));
}

/*
 * Engine parameters spec is a poor-man's JSON.  name:value pairs are separated
 * by commas.  White space is trimmed off front and back.  No quotes or anything
 * fancy allowed.
 */
static adios2::Params ParseEngineParams(std::string Input)
{
    std::istringstream ss(Input);
    std::string Param;
    adios2::Params Ret = {};

    while (std::getline(ss, Param, ','))
    {
        std::istringstream ss2(Param);
        std::string ParamName;
        std::string ParamValue;
        std::getline(ss2, ParamName, ':');
        if (!std::getline(ss2, ParamValue, ':'))
        {
            throw std::invalid_argument("Engine parameter \"" + Param +
                                        "\" missing value");
        }
        Ret[Trim(ParamName)] = Trim(ParamValue);
    }
    return Ret;
}

static void ParseArgs(int argc, char **argv)
{
    int bare_arg = 0;
    while (argc > 1)
    {
        if (std::string(argv[1]) == "--expect_time_gap")
        {

            TimeGapExpected++;
            IgnoreTimeGap = 0;
        }
        else if (std::string(argv[1]) == "--expect_contiguous_time")
        {
            TimeGapExpected = 0;
            IgnoreTimeGap = 0;
        }
        else if (std::string(argv[1]) == "--compress_sz")
        {
            CompressSz++;
        }
        else if (std::string(argv[1]) == "--shared_io")
        {
            SharedIO = true;
        }
        else if (std::string(argv[1]) == "--shared_var")
        {
            SharedVar = true;
            SharedIO = true;
        }
        else if (std::string(argv[1]) == "--compress_zfp")
        {
            CompressZfp++;
        }
        else if (std::string(argv[1]) == "--filename")
        {
            fname = std::string(argv[2]);
            argv++;
            argc--;
        }
        else if (std::string(argv[1]) == "--write_mode")
        {
            if (strcasecmp(argv[2], "sync") == 0)
            {
                GlobalWriteMode = adios2::Mode::Sync;
            }
            else if (strcasecmp(argv[2], "deferred") == 0)
            {
                GlobalWriteMode = adios2::Mode::Deferred;
            }
            else
            {
                std::cerr << "Invalid mode for --write_mode " << argv[2]
                          << std::endl;
            }
            argv++;
            argc--;
        }
        else if (std::string(argv[1]) == "--engine_params")
        {
            engineParams = ParseEngineParams(argv[2]);
            argv++;
            argc--;
        }
        else if (std::string(argv[1]) == "--engine")
        {
            engine = std::string(argv[2]);
            argv++;
            argc--;
        }
        else if (std::string(argv[1]) == "--expect_timeout")
        {
            ExpectOpenTimeout++;
        }
        else if (std::string(argv[1]) == "--duration")
        {
            std::istringstream ss(argv[2]);
            if (!(ss >> DurationSeconds))
                std::cerr << "Invalid number for duration " << argv[1] << '\n';
            argv++;
            argc--;
        }
        else if (std::string(argv[1]) == "--shutdown_filename")
        {
            shutdown_name = std::string(argv[2]);
            argv++;
            argc--;
        }
        else if (std::string(argv[1]) == "--ms_delay")
        {
            std::istringstream ss(argv[2]);
            if (!(ss >> DelayMS))
                std::cerr << "Invalid number for ms_delay " << argv[1] << '\n';
            argv++;
            argc--;
        }
        else if (std::string(argv[1]) == "--zero_data_var")
        {
            ZeroDataVar++;
        }
        else if (std::string(argv[1]) == "--zero_data_rank")
        {
            ZeroDataRank++;
        }
        else
        {
            if (bare_arg == 0)
            {
                /* first arg without -- is engine */
                engine = std::string(argv[1]);
                bare_arg++;
            }
            else if (bare_arg == 1)
            {
                /* second arg without -- is filename */
                fname = std::string(argv[1]);
                bare_arg++;
            }
            else if (bare_arg == 2)
            {
                engineParams = ParseEngineParams(argv[1]);
                bare_arg++;
            }
            else
            {

                throw std::invalid_argument("Unknown argument \"" +
                                            std::string(argv[1]) + "\"");
            }
        }
        argv++;
        argc--;
    }
}
