#ifndef DATA_DELETE_H
#define DATA_DELETE_H

#include "../Data_reading/root_read.h"
#include "../Boot_reading/boot.h"
#include <iostream>
#include <string>
#include <map>
#include <cmath>

struct NODE
{
    int cluster_number;
    NODE *next;
};

class data_delete : public root_read
{
private:
    NODE file_clusters;
    boot_record boot;
    root_directory root;
    void get_file_clusters(FILE *partition);
    void print_file_clusters(FILE *partition);
    void choose_file_delete(FILE *partition);
    void free_bitmap(FILE *partition);
    void delete_metadata(FILE *partition, int archiveIndex);
    void flip_bit(int mode, int position, FILE *partition);
    void set_data_type(char data_type, FILE *partition);

public:
    data_delete();
    ~data_delete();
    data_delete(FILE *partition, boot_record boot);

    void delete_file(FILE *partition);
};

#endif // DATA_DELETE_H
