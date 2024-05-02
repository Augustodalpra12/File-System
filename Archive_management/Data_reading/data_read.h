#include <string>
#include <iostream>
#include "../Boot_reading/boot.h"
using namespace std;
class data_read
{
private:
    char data_type;
    short time_created;
    short date_created;
    short last_access;
    short time_modified;
    short date_modified;
    int first_cluster;
    int file_size;
    char file_name[13];
public:
    data_read(/* args */);
    ~data_read();
    data_read(FILE* file);

    void read_data(FILE* file, int archive);
    void print_data_type();
    void print_time_created();
    void print_date_created();
    void print_last_access();
    void print_time_modified();
    void print_date_modified();
    void print_first_cluster();
    void print_file_size();
    void print_file_name();
    void print_archive_info();

    int get_data_type();
    int get_time_created();
    int get_date_created();
    int get_last_access();
    int get_time_modified();
    int get_date_modified();
    int get_first_cluster();
    int get_file_size();
    string get_file_name(); 


    int search_data(FILE* file, string name);
    void read_archive(FILE* file);
    void read_all_files(FILE* file);

}__attribute__((packed));
