#include "formater.h"
#include <iostream>
#include <fstream>

using namespace std;

// REFATORA ISSO ANTES DE ENVIAR POR QUE TA MUITO FEIO JESUS CRISTO AMADO

int main()
{
    formater new_disk = create_new_disk();
    if(!new_disk.archive_is_open())
    {
        cout << "Error creating the archive" << endl;
        return 0;

    }
    cout << "Insert a name for the disk partition with a maximum of 8 characters: " << endl;
    char partition_name[8];
    cin.get(partition_name, 8);

    cout << "Do you want to especify a size for the sectors? y/n" << endl;
    char response;
    do
    {
        cin >> response;

    } while (!(response == 'y' || response == 'n'));

    int bytes_per_sector = 512;
    if (response == 'y')
    {
        cout << "How many bytes will a sector have?" << endl;
        cin >> bytes_per_sector;
    }

    cout << "Do you want to especify how many sectors per cluster? y/n" << endl;
    do
    {
        cin >> response;
    } while (!(response == 'y' || response == 'n'));

    int sectors_per_cluster = 1;
    if (response == 'y')
    {
        cout << "How many sectors will a cluster have?" << endl;
        cin >> sectors_per_cluster;
    }

    cout << "Formatando partição";
    new_disk.format_partition(partition_name, sectors_per_cluster, bytes_per_sector);

    return 0;
}

formater create_new_disk()
{
    string filename = get_file_name();
    int file_size_mb = get_file_size();

    formater newDisk(filename, file_size_mb);
    return newDisk;
}

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
    cout << "Inser the size of the partition (in mb): " << endl;
    int file_size_mb;
    cin >> file_size_mb;

    return file_size_mb;
}

char* get_partition_name(){

}
