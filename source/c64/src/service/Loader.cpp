#include "Loader.h"
namespace Service
{
/**
 * loads binary files and put them into c64 memory
 */

Loader::Loader(MemoryManager* memManager)
{
    this->memManager  = memManager;
}
Loader::~Loader()
{
    //dtor
}
void Loader::loadConfig()
{
    streampos size;

    ifstream osFile ("config.txt");
    if (osFile.is_open())
    {

        std::string line;
        while (std::getline(osFile, line))
        {

            if(line.find("delayForSid")!= string::npos)
            {
                vector<string> arr = splitString(line,' ');
                Config::getInstance().delayForSid = stoi(arr[1]);
            }
            else if(line.find("load") != string::npos)
            {
                vector<string> arr = splitString(line,' ');
                int address = stoi(arr[1],nullptr,16);
                string filename = arr[2];
                this->load(address,filename);
            }
            line ="";

        }

    }


}

/*
 * loads a binary file and stores it in the c64 memory
 */
bool Loader::load(int address,string filename)
{
     streampos size;


    ifstream osFile (filename.c_str(), ios::in|ios::binary|ios::ate);
    if (osFile.is_open())
    {
        size = osFile.tellg();
        unsigned char* buffer = new unsigned char[size];
        osFile.seekg (0, ios::beg);
        osFile.read ((char*)buffer, size);
        osFile.close();

        cout << "the entire file content is in memory";
        binToChar(buffer,address,size);
        delete buffer;
        return true;
    }
    else
    {
        cout << "something goes wrong..." <<endl;
        return false;
    }

}
/*
 * convert the binary in the ugly string array format
 */
void Loader::binToChar(unsigned char* bin,int address,int length)
{

    for(int i = 0; i < length;i++)
    {
        memManager->assignValue(bin[i],address+i);
    }

}

std::vector<string>Loader::splitString(string line,char delimeter)
{
    vector<string>strArray;
    std::istringstream iss(line);

    string tmp;
    while (std::getline(iss, tmp,' '))
    {
        strArray.push_back(tmp);
    }
    return strArray;

}

}
