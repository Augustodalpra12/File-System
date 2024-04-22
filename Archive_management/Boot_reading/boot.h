#include <string>
#include <iostream>
using namespace std;

class boot_record
{
private:
    char partition_name[8];
    short bytes_per_sector;
    char sectors_per_cluster;
    short root_entry_count;
    int bitmap_in_clusters;

public:

    boot_record();
    ~boot_record();
    boot_record(FILE* file);
    void read_boot_record(FILE* file);

    void print_partition_name();
    void print_bytes_per_sector();
    void print_sectors_per_cluster();
    void print_root_entry_count();
    void print_bitmap_in_clusters();

    string get_partition_name();
    int get_bytes_per_sector();
    int get_sectors_per_cluster();
    int get_root_entry_count();
    int get_bitmap_in_clusters();


}__attribute__((packed));
