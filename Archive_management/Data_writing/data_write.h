#ifndef DATA_WRITE_H
#define DATA_WRITE_H
#include <string>
#include <iostream>
#include <cmath>
#include <ctime>
#include "../Boot_reading/boot.h"
#include "../Assets/assets.h"
using namespace std;
class data_write
{
private:
    FILE* partition;
    FILE* file_to_copy;
    string file_name;
    File_name name_to_root;
    int file_size;
    int bitmap_start;
    NODE file_clusters;
    boot_record boot;

    // -- Methods (order of use) -- 
    int get_file();
    void get_filename();
    File_name file_name_fromextension(string fullname);
    int is_name_valid(File_name file_name);

    void get_file_size();

    int check_available_clusters();
    int count_free_bits(int index, char byte, int spaces_needed);

    int set_occupied_bits();

    void write_file();

    int write_on_root();
    int search_root_space();

    void set_data_type(char data_type);

    tm* get_now();

    void set_creation(Date_Hour today);
    void set_last_access(Date_Hour today);
    void set_modification(Date_Hour today);
    void set_first_cluster();
    void set_file_size();
    void set_filename();

    int flip_bit(int mode, int position);
    void add_cluster(int cluster_number);

    short pack_date(int year, int month, int day);
    short pack_time(int hour, int minute, int second);

    int cluster_to_bitmap_position(int cluster_number);
    int bitmap_position_to_cluster(int bit);

public:
    data_write(boot_record boot, FILE* partition);
    ~data_write();

};

#endif