#include "./Boot_reading/boot.h"
#include "./Data_writing/data_write.h"
#include "./Data_reading/data_read.h"
// g++ main.cpp .\Boot_reading\boot.cpp .\Data_reading\data_read.cpp .\Data_writing\data_write.cpp -O main.cpp

int main(int argc, char const *argv[])
{
    cout << "Enter file name with extension: ";
    string filename = "../";

    string aux;
    cin >> aux;

    FILE* partition;
    filename += aux;

    partition = fopen(filename.c_str(), "rb");
    while (partition == NULL)
    {
        cout << "File not found. Enter a valid file name" << endl;
        cin >> aux;
        filename = "../";
        filename += aux;
        partition = fopen(filename.c_str(), "rb");
    }

    boot_record boot;
    boot.read_boot_record(partition);
    boot.print_partition_name();
    boot.print_bytes_per_sector();
    boot.print_sectors_per_cluster();
    boot.print_root_entry_count();
    boot.print_bitmap_in_clusters();
    return 0;
}
