#include <iostream>
#include <cstring>
#include <fstream>
#include <cmath>


using namespace std;

struct boot_record
{
    char partition_name[8];
    unsigned short bytes_per_sector;
    unsigned char sectors_per_cluster;
    unsigned short root_entries;
    unsigned int bitmap_size;
}__attribute__((packed));

class formater
{
private:
    boot_record boot;
    int root_in_sectors;

    int bitmap_start;
    int partition_size_in_bytes;
    string archive_name;

    fstream partition;

    int define_boot_record(int sectors_per_cluster = 1, int bytes_per_sector = 512);

    int calc_root_entries();
    int calc_bitmap_size();
   
    int reset_reserved_partitions();
    int reset_root_dir();

    int set_bitmap();

    int get_root_dir_in_sectors();
public:
    formater(const string& filename);
    formater(const formater& F);
    ~formater();
    int create_partition_file();
    int expand_file_size();

    int archive_is_open();
    int format_partition(int sectors_per_cluster = 1, int bytes_per_sector = 512);
    
    void set_filename(string filename);
    void set_file_size(int filesize);
    void set_partition_name(char partition_name[8]);

    void write_test(string text);
    void write_boot_record();
};