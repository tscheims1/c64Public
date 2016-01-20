#include "Config.h"

/**
 * The config class uses the singleton pattern
 */
namespace Service
{
Config::Config()
{

}
Config& Config::getInstance()
{
    static Config instance;

    return instance;
}
Config::~Config()
{
}
};
