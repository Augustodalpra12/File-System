#include "formater.h"
struct partition_name
{
    char name[8];
};


string get_file_name()
{
    string full_name;
    cout << "Insert the name of the new file: " << endl;
    string filename;
    cin >> filename;

    full_name = filename;
    full_name += ".img";
    
    return full_name;
}

int get_file_size()
{
    long file_size_mb;
    cout << "Insert the size of the partition (in mb): " << endl;
    cin >> file_size_mb;

    return file_size_mb;
}

partition_name get_partition_name()
{
    cout << "Insert the name of the partition: " << endl;
    partition_name partition_name;
    cin >> partition_name.name;
    return partition_name;
}

int response_is_valid(char response)
{
    return (response == 'y' || response == 'n');
}

int specify_bytes_per_sector()
{
    return 512;
    cout << "Do you want to especify a size for the sectors? y/n" << endl;
    char response;
    do
    {
        cin >> response;
    } while (!response_is_valid(response));
    
    int bytes_per_sector = 512;
    if (response == 'y')
    {
        cout << "How many bytes will a sector have?" << endl;
        cin >> bytes_per_sector;
    }
    return bytes_per_sector;
}

int specify_sector_per_cluster()
{
    return 1;
    cout << "Do you want to especify how many sectors per cluster? y/n" << endl;
    char response;
    do
    {
        cin >> response;
    } while (!response_is_valid(response));

    int sectors_per_cluster = 1;
    if (response == 'y')
    {
        cout << "How many sectors will a cluster have?" << endl;
        cin >> sectors_per_cluster;
    }

    return sectors_per_cluster;
}

formater::formater()
{
    this->archive_name = get_file_name();
    create_partition_file();
    if(!this->archive_is_open())
    {
        cout << "Error creating the file" << endl;
        return;
    }

    this->set_file_size(get_file_size());
    this->expand_file_size();

    partition_name partition = get_partition_name();

    this->set_partition_name(partition.name);

    int sectors_per_cluster = specify_sector_per_cluster();
    int bytes_per_sector = specify_bytes_per_sector();
    
    cout << "Formating partition..." << endl;
    this->format_partition(sectors_per_cluster, bytes_per_sector);

    cout << "Disk formated!" << endl;
}

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
    this->create_partition_file();

}



formater::~formater()
{
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
    boot.bitmap_size_in_clusters = calc_bitmap_size();

    return 0;
}

int formater::calc_root_entries()
{
    int root_entries = (partition_size_in_bytes/(boot.bytes_per_sector * static_cast<int>(boot.sectors_per_cluster)) - 3);
    root_in_sectors = ceil((root_entries * 32.0) / boot.bytes_per_sector);
    return root_entries;
}

int formater::calc_bitmap_size()
{
    float bytes_per_cluster = boot.bytes_per_sector * static_cast<int>(boot.sectors_per_cluster);

    int partition_total_clusters = floor(partition_size_in_bytes/bytes_per_cluster);

    int bitmap_size_in_bytes = ceil(partition_total_clusters / 8.0);

    int bitmap_in_cluster = ceil(bitmap_size_in_bytes/bytes_per_cluster);

    return bitmap_in_cluster;
}
    
int formater::reset_reserved_partitions()
{
    reset_root_dir();
    set_bitmap();

    return 0;
}

int formater::reset_root_dir()
{
    partition.seekp(boot.bytes_per_sector, ios::beg);
    char invalid = static_cast<char>(stoi("FF", nullptr, 16));
    for (int i = 0; i < boot.root_entries; i++)
    {
        partition.write(&invalid, 1);
        partition.seekp(31, ios::cur);
    }
    return 0;
}

int formater::set_bitmap()
{
    int bitmap_start = boot.bytes_per_sector * (root_in_sectors + 1);
    

    int reserved_clusters = ceil((root_in_sectors + 1) / static_cast<int>(boot.sectors_per_cluster)) + boot.bitmap_size_in_clusters;

    partition.seekp(bitmap_start, ios::beg);
    if (reserved_clusters > 8)
    {
        write_occupied_bytes(reserved_clusters);
    }
    
    int leftover_bits = reserved_clusters%8;
    if (leftover_bits)
    {
        write_remaining_bits(leftover_bits);
    }

    int usable_clusters = partition_size_in_bytes/(boot.bytes_per_sector * static_cast<int>(boot.sectors_per_cluster));

    int total_cluster_quantity = usable_clusters/8;
    partition.seekp(bitmap_start, ios::beg); // Vai pro começo do bitmap
    partition.seekp((total_cluster_quantity), ios::cur); // Vai pro final da parte útil do bitmap


    int invalid_spaces = (boot.bitmap_size_in_clusters*(boot.bytes_per_sector * static_cast<int>(boot.sectors_per_cluster))) - total_cluster_quantity; 

    int bitmap_final_byte = usable_clusters%8;


    if (bitmap_final_byte)
    {
        char one = final_bitmap_bits(bitmap_final_byte);
        partition.write(&one, 1);
        invalid_spaces--;
    }

    // Pega o tamanho do bitmap e ve o espaço que ta sobrando no cluster
    // NÃO TA FAZENDO CERTO ISSO, TA CORTANDO UM BYTE NO FINAL, PERCA DE 1 A 8 CLUSTERS
    
    char blank = 0xFF;
    for (int i = 0; i < invalid_spaces; i++)
    {
        partition.write(&blank, 1);
    }
    

    return 0;
}

int formater::write_occupied_bytes(int reserved_clusters)
{
    int bytes_to_write = reserved_clusters/8;
    char full_byte = 255;
    for (int i = 0; i < bytes_to_write; i++)
    {
        partition.write(&full_byte, 1);
    }

    return 0;
}

int formater::write_remaining_bits(int leftover_clusters)
{
    char mask = (~0 << (8 - leftover_clusters));
    char bits_to_write = 255 & mask;

    partition.write(&bits_to_write, 1);
    return 0;
}

char formater::final_bitmap_bits(int remaining_bits)
{
    char mask = (~0 << (8 - remaining_bits));
    char bits_to_write = 255 & mask;

    return (~bits_to_write);
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
    if (partition.is_open()) { // Check if the file is open
        partition << text << endl;
    } else {
        cout << "File is not open." << endl;
    }
}

void formater::write_boot_record()
{
    // partition.write((char*)&boot, sizeof(boot)); // Não ta funcionando, escrevendo lixo(?????????)
    partition.write((char*)&boot.partition_name, 8);
    partition.write((char*)&boot.bytes_per_sector, 2);
    partition.write((char*)&boot.sectors_per_cluster, 1);
    partition.write((char*)&boot.root_entries, 2);
    partition.write((char*)&boot.bitmap_size_in_clusters, 4);
}

string formater::get_filename()
{
    return archive_name;
}