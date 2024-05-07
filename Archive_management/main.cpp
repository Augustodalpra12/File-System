#include "./Boot_reading/boot.h"
#include "./Data_writing/data_write.h"
#include "./Data_delete/data_delete.h"
/*g++ main.cpp
.\Boot_reading\boot.cpp
.\Data_reading\root_read.cpp
.\Data_writing\data_write.cpp .\Data_writing\file_name.cpp
.\Data_delete\data_delete.cpp
.\Assets\assets.cpp
*/
// g++ main.cpp .\Boot_reading\boot.cpp .\Data_reading\root_read.cpp .\Data_writing\data_write.cpp .\Data_writing\file_name.cpp .\Data_delete\data_delete.cpp -o main

int main(int argc, char const *argv[])
{
    cout << "Enter file name with extension: ";
    string filename = "../";

    string aux;
    cin >> aux;

    FILE *partition;
    filename += aux;

    partition = fopen(filename.c_str(), "r+b");
    while (partition == NULL)
    {
        cout << "File not found. Enter a valid file name" << endl;
        cin >> aux;
        filename = "../";
        filename += aux;
        partition = fopen(filename.c_str(), "r+b");
    }

    boot_record boot;
    boot.read_boot_record(partition);
    boot.print_partition_name();
    boot.print_bytes_per_sector();
    boot.print_sectors_per_cluster();
    boot.print_root_entry_count();
    boot.print_bitmap_in_clusters();

    data_delete data_deletar(partition, boot);
    int option = 0;

    while (true)
    {
        cout << "1. Write file" << endl;
        cout << "2. Delete file" << endl;
        cout << "3. Read file" << endl;
        cout << "4. Exit" << endl;
        cout << "Option: ";
        cin >> option;

        if (option == 1)
        {
            data_write data(boot, partition);

        } else if (option == 2) {
            data_deletar.delete_file(partition);

        } else if (option == 3) {
            root_read root(partition);
            
        } else if (option == 4) {
            return 0;

        } else {

            cout << "Invalid option, try again" << endl;
        }
        
    }
}
