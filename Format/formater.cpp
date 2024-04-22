#include "formater.h"

formater::formater(string archive_name, int archive_size)
{
    this->archive_name = archive_name;
    this->partition_size_in_bytes = archive_size;
    if (!create_partition_file())
        return;
    expand_file_size(this->partition_size_in_bytes);    
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
    {
        return 1;
    }
    
    return 0;
}

int formater::create_partition_file()
{
    partition.open(archive_name, fstream::in | fstream::binary);
    if (!archive_is_open)
        return 0;
    return 1;
}

int formater::expand_file_size(int n)
{
    partition.seekp(n * 1024 * 1024);
    return 0;
}

int formater::define_boot_record(char partition_name[8], int sectors_per_cluster, int bytes_per_sector)
{
    // boot.partition_name = partition_name;
    boot.bytes_per_sector = bytes_per_sector;
    boot.sectors_per_cluster = sectors_per_cluster;
    boot.root_entries = calc_root_entries();
    // boot.bitmap_size = 0; //Separe this in a function -> ceil(ceil(partition_size/bytes_per_sector)/sectors_per_cluster);
}

int formater::calc_root_entries()
{

    return 0;
}
    
int formater::reset_reserved_partitions()
{
    reset_root_dir();
    set_bitmap();
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