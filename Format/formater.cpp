#include "formater.h"

formater::formater()
{   
}

formater::formater(const formater& F)
{
    this->boot = F.boot;
    this->partition_size_in_bytes = F.partition_size_in_bytes;
    this->root_in_clusters = F.root_in_clusters;
    this->archive_name = F.archive_name;
    if (!this->create_partition_file())
        return;
    // this->partition = move(F.partition); // Trying to copy this F* fstream but it's a piece of shit
}

formater::~formater()
{
    if (partition.is_open())
    {
        partition.close();
    }
}

int formater::archive_is_open()
{
    if (partition.is_open())
        return 1;
    return 0;
}

int formater::create_partition_file()
{
    
    partition.open(archive_name, ios::in | ios::out | ios::trunc);
    if (!archive_is_open())
        return 0;
    return 1;
}

int formater::expand_file_size(int n)
{
    partition.seekp(n * 1024 * 1024);
    return 0;
}

int formater::format_partition(int sectors_per_cluster, int bytes_per_sector)
{
    define_boot_record(sectors_per_cluster, bytes_per_sector);
    reset_reserved_partitions();
    return 0;
}

int formater::define_boot_record(int sectors_per_cluster, int bytes_per_sector)
{
    boot.bytes_per_sector = bytes_per_sector;
    boot.sectors_per_cluster = sectors_per_cluster;
    boot.root_entries = calc_root_entries();
    // boot.bitmap_size = 0; //Separe this in a function -> ceil(ceil(partition_size/bytes_per_sector)/sectors_per_cluster);

    return 0;
}

int formater::calc_root_entries()
{
    int root_entries = (partition_size_in_bytes/(boot.bytes_per_sector * boot.sectors_per_cluster) - 3);
    return root_entries;
}
    
int formater::reset_reserved_partitions()
{
    reset_root_dir();
    set_bitmap();

    return 0;
}

int formater::reset_root_dir()
{
    
    for (int i = 0; i < boot.root_entries; i++)
    {
        // Write 0xFF on the first byte, don't touch the rest
        // Move 32 bytes forward
    }
    return 0;
}

int formater::set_bitmap()
{
    int i = 0;
    for (i; i < 3; i++) // 3 Vai ser substituido por um valor que se refere a quantidade de setores reservados (1 + root_in_cluster + bitmap_in_cluster)
    {
        // Inserir estes setores como preenchidos
    }

    for (i; i < 3; i++) // 3 Vai ser substituido pela quantidade total de clusters do disco.
    {

    }

    // Um novo for preenchendo o espaço vazio ao final do cluster do bitmap colocando todos esses espaços, que no caso são invalidos, como preenchidos.
    return 0;
}

void formater::set_filename(string filename)
{
    archive_name = filename;
}

void formater::set_file_size(int filesize)
{
    partition_size_in_bytes = filesize;
}

void formater::set_partition_name(char partition_name[8])
{
    strcpy(boot.partition_name, partition_name);
}

void formater::write_test(string text)
{
    partition << text;
}