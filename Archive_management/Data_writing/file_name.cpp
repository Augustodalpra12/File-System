#include "file_name.h"

int File_name::get_name_size()
{
    return name_size;
}

int File_name::get_extension_size()
{
    return extension_size;
}

string File_name::get_name()
{
    return name;
}

string File_name::get_extension()
{
    return extension;
}

void File_name::set_name(string fullname)
{
    int dot = fullname.find_last_of(".");

    if (dot != string::npos)
    {
        this->name = fullname.substr(0, dot);
        this->extension = fullname.substr(dot + 1);

        this->name_size = name.size();
        this->extension_size = extension.size();
    } 
    else
    {
        this->name = fullname;
        this->extension = "";

        this->name_size = name.size();
        this->extension_size = 0;
    }
}


int File_name::is_name_valid(){
    if (get_name_size() > 10)
        return 0;
    
    if (get_extension_size() > 3)
        return 0;

    return 1;
}
