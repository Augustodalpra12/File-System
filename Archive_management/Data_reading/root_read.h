#include <string>
#include <iostream>
#include "../Boot_reading/boot.h"
#include <map>
#include <climits>
#include <ctime>

using namespace std;

// struct Date_Hour
// {
//     short date;
//     short time;
// };

struct root_directory
{
    char data_type;
    short time_created;
    short date_created;
    short last_access;
    short time_modified;
    short date_modified;
    int first_cluster;
    int file_size;
    char file_name[13];

} __attribute__((packed));
class root_read
{
private:
    map<int, string> root_directory_map;
    root_directory root;

public:
    root_read(/* args */);
    ~root_read();
    root_read(FILE *file);

    void read_data(FILE *file);
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
    int get_valid_index();
    int is_index_valid(int index);
    // void update_last_access(FILE *partition);
    // short pack_date(int year, int month, int day);
    // short pack_time(int hour, int minute, int second);
    // void set_last_access(Date_Hour today);
    root_directory get_root();
    int get_data_type();
    int get_time_created();
    int get_date_created();
    int get_last_access();
    int get_time_modified();
    int get_date_modified();
    int get_first_cluster();
    int get_file_size();
    string get_file_name();
    // tm *get_now();
    void search_data(FILE *file, int index);
    void read_archive(FILE *file);
    void read_all_files(FILE *file);
};