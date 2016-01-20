#ifndef CONFIG_H
#define CONFIG_H

namespace Service
{
/**
 * Singleton for storing the Configuration
 */
class Config
{
    public:
        int delayForSid = 0;
        virtual ~Config();
        static Config& getInstance();
    private:

        Config();
};
};
#endif // CONFIG_H
