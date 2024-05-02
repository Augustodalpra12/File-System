#include <string>
#include <iostream>
#include <cmath>
#include "../Boot_reading/boot.h"
using namespace std;

struct NODE
{
    int cluster_number;
    NODE* next;
};

class data_write
{
private:
    FILE* partition;
    FILE* file_to_copy;
    string file_name;
    int file_size;
    int bitmap_start;
    NODE file_clusters;
    boot_record boot;

    void get_file_size();

    int check_available_clusters();
    int count_free_bits(int index, char byte);

    NODE* serach_neadeed_clusters();
    int set_occupied_bits();
    int bitmap_position_to_cluster(int bit);

    int cluster_to_bitmap_position(int cluster_number);

    int get_file();

    void get_filename();
    void set_data_type(int data_type);
    void set_creation();
    void set_last_access();
    void set_modification();
    void set_first_cluster();
    void set_file_size();
    void set_filename();

    void write_file();

    int flip_bit(int mode, int position);

    void add_cluster(int cluster_number);

public:
    data_write(boot_record boot, FILE* partition);
    ~data_write();

};
