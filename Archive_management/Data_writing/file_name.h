#ifndef FILE_NAME_H
#define FILE_NAME_H
#include <iostream>
#include <string>

using namespace std;

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