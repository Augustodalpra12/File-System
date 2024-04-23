#include "formater.h"

formater::formater(const string& filename)
{   
    archive_name = filename;
    partition.open(archive_name, ios::out | ios::trunc);
}

formater::formater(const formater& F)
{
    this->boot = F.boot;
    this->partition_size_in_bytes = F.partition_size_in_bytes;
    this->root_in_sectors = F.root_in_sectors;
    this->archive_name = F.archive_name;
    if (!this->create_partition_file())
        return;
}

formater::~formater()
{
    cout << "Deleting formater." << endl;
    if (partition.is_open())
        partition.close();
}

int formater::archive_is_open()
{
    if (partition.is_open())
        return 1;
    return 0;
}

int formater::create_partition_file()
{
    partition.open(archive_name, ios::out | ios::trunc);
    if (!archive_is_open())
        return 0;

    return 1;
}

int formater::expand_file_size()
{
    partition.seekp(partition_size_in_bytes - 1, ios::beg);
    partition.write("", 1);
    partition.seekp(0, ios::beg);
    return 0;
}

int formater::format_partition(int sectors_per_cluster, int bytes_per_sector)
{
    define_boot_record(sectors_per_cluster, bytes_per_sector);
    write_boot_record();
    reset_reserved_partitions();
    return 0;
}

int formater::define_boot_record(int sectors_per_cluster, int bytes_per_sector)
{
    boot.bytes_per_sector = bytes_per_sector;
    boot.sectors_per_cluster = sectors_per_cluster;
    boot.root_entries = calc_root_entries();
    boot.bitmap_size = calc_bitmap_size(); //Separe this in a function -> ceil(ceil(partition_size/bytes_per_sector)/sectors_per_cluster);

    return 0;
}

int formater::calc_root_entries()
{
    int root_entries = (partition_size_in_bytes/(boot.bytes_per_sector * static_cast<int>(boot.sectors_per_cluster)) - 3);
    root_in_sectors = ceil((root_entries * 32) / boot.bytes_per_sector);
    return root_entries;
}

int formater::calc_bitmap_size()
{
    int bitmap_size_in_bytes = ceil(ceil(partition_size_in_bytes/(boot.bytes_per_sector * static_cast<int>(boot.sectors_per_cluster))) / 8);

    return ceil(bitmap_size_in_bytes/(boot.bytes_per_sector * static_cast<int>(boot.sectors_per_cluster)));
}
    
int formater::reset_reserved_partitions()
{
    reset_root_dir();
    cout << "Resetting root dir." << endl;
    set_bitmap();

    return 0;
}

int formater::reset_root_dir()
{
    partition.seekp(boot.bytes_per_sector, ios::beg);
    char invalid = static_cast<char>(stoi("FF", nullptr, 16));
    for (int i = 0; i < boot.root_entries; i++)
    {
        // Write 0xFF on the first byte, don't touch the rest
        partition.write(&invalid, 1);
        // Move 32 bytes forward
        partition.seekp(31, ios::cur);
    }
    return 0;
}

int formater::set_bitmap()
{
    int bitmap_start = boot.bytes_per_sector * (root_in_sectors + 1);
    
    partition.close();

    partition.open(archive_name, ios::out | ios::binary);
    
    partition.seekp(bitmap_start, ios::beg);

    

    int reserved_clusters = ceil((root_in_sectors + 1) / static_cast<float>(boot.sectors_per_cluster)) + boot.bitmap_size;
    int i = 0;
    for (i; i < reserved_clusters; i++)
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
    cout << "Filesize on the formater: " << filesize << endl;
    partition_size_in_bytes = filesize;
}

void formater::set_partition_name(char partition_name[8])
{
    strcpy(boot.partition_name, partition_name);
}

void formater::write_test(string text)
{
    if (partition.is_open()) { // Check if the file is open
        partition << text << endl;
    } else {
        cout << "File is not open." << endl;
    }
}

void formater::write_boot_record()
{
    partition.write((char*)&boot, sizeof(boot));
}