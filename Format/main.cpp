#include "formater.h"
#include <iostream>
#include <fstream>

using namespace std;

// REFATORA ISSO ANTES DE ENVIAR POR QUE TA MUITO FEIO JESUS CRISTO AMADO

string get_file_name()
{
    cout << "Insert the name of the new file: " << endl;
    string filename;
    cin >> filename;

    filename += ".img";
    
    return filename;
}

int get_file_size()
{
    cout << "Insert the size of the partition (in mb): " << endl;
    int file_size_mb;
    cin >> file_size_mb;

    return file_size_mb;
}

int create_new_disk(formater new_disk)
{
    string filename = get_file_name();
    int file_size_mb = get_file_size();

    new_disk.set_filename(filename);
    if(!new_disk.create_partition_file())
    {
        return 0;
    }

    new_disk.set_file_size(file_size_mb);
    new_disk.expand_file_size(file_size_mb);
    return 1;
}

void get_partition_name(formater new_disk)
{
    cout << "Insert the name of the partition: " << endl;
    char partition_name[8];
    cin >> partition_name;
    new_disk.set_partition_name(partition_name);
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
    formater new_disk;

    if(!create_new_disk(new_disk))
    {
        cout << "Error creating the archive" << endl;
        return 0;
    }

    get_partition_name(new_disk);


    int sectors_per_cluster = specify_sector_per_cluster();
    int bytes_per_sector = specify_bytes_per_sector();

    new_disk.write_test("Hello World");
    
    cout << "Formatando partição";
    new_disk.format_partition(sectors_per_cluster, bytes_per_sector);

    new_disk.~formater();

    return 0;
}