#include "formater.h"
#include <iostream>
#include <fstream>

using namespace std;

// REFATORA ISSO ANTES DE ENVIAR POR QUE TA MUITO FEIO JESUS CRISTO AMADO

struct partition_name
{
    char name[8];
};


string get_file_name()
{
    string full_name = "..\\";
    cout << "Insert the name of the new file: " << endl;
    string filename;
    cin >> filename;

    full_name += filename;
    full_name += ".img";
    
    return full_name;
}

int get_file_size()
{
    int file_size_mb;
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

int main()
{
    formater new_disk(get_file_name());
    if(!new_disk.archive_is_open())
    {
        cout << "Error creating the file" << endl;
        return 0;
    }

    new_disk.set_file_size(get_file_size());
    new_disk.expand_file_size();

    partition_name partition = get_partition_name();

    new_disk.set_partition_name(partition.name);

    int sectors_per_cluster = specify_sector_per_cluster();
    int bytes_per_sector = specify_bytes_per_sector();
    
    cout << "Formatando partição" << endl;
    new_disk.format_partition(sectors_per_cluster, bytes_per_sector);

    return 0;
}