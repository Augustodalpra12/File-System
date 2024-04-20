#include <iostream>
#include <fstream>

using namespace std;

struct boot_record
{
    char[8] partition_name
    int bytes_per_sector;
    int sector_per_cluster;
    int root_entries;
    int bitmap_size;
}__attribute__((packed));

class formater
{
private:
    boot_record boot;
    int partition_size_in_bytes;

    FILE partition;

public:
    formater();
    ~formater();

    int define_boot_record(char[8] partition_name, int partition_size, int sector_per_cluster = 1, int bytes_per_sector = 512);
    int calc_root_entries();
    int reset_reserved_partitions();
    int reset_root_dir();
    int set_bitmap();
};

formater::format(FILE archive)
{
    partition = archive;
    partition.seekp(0);
}

formater::~format()
{

}

int formater::define_boot_record(char[8] partition_name, int partition_size, int sector_per_cluster = 1, int bytes_per_sector = 512)
{
    boot.partition_name = partition_name;
    boot.bytes_per_sector = bytes_per_sector;
    boot.sector_per_cluster = sector_per_cluster;
    boot.root_entries = calc_root_entries(int partition_size);
}

int formater::calc_root_entries(int partition_size)
{
    
}
    
int formater::reset_reserved_partitions()
{
    reset_root_dir();
    set_bitmap();

}

int formater::reset_root_dir()
{
    for (int i = 0; i < root_entries; i++)
    {
        // Write 0xFF on the first byte, don't touch the rest
        // Move 32 bytes forward
    }
}

int formater::set_bitmap()
{
    int root_in_clusters = (root_entries * 32) / sector
    for (size_t i = 0; i < count; i++)
    {
        /* code */
    }

}