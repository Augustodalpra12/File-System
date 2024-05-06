#ifndef ASSETS
#define ASSETS

#include <iostream>
#include <cstring>

using namespace std;

struct NODE
{
    int cluster_number;
    NODE* next;
};

struct Date_Hour
{
    short date;
    short time;
};

short pack_date(int year, int month, int day);
short pack_time(int hour, int minute, int second);

class File_name
{
private:
    string name;
    string extension;
    int name_size;
    int extension_size;

    void set_name_size(string name);
    void set_extension_size(string extension);

public:
    int get_name_size();
    int get_extension_size();

    string get_name();
    string get_extension();

    void set_name(string fullname);

    int is_name_valid();    
};

#endif