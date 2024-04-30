#include "boot.h"

boot_record::boot_record(){
    
}

boot_record::boot_record(const boot_record& boot){
    strcpy(this->partition_name, boot.partition_name);
    this->bytes_per_sector = boot.bytes_per_sector;
    this->sectors_per_cluster = boot.sectors_per_cluster;
    this->root_entry_count = boot.root_entry_count;
    this->bitmap_in_clusters = boot.bitmap_in_clusters;
}
boot_record::boot_record(FILE* file){
    this->read_boot_record(file);
}

boot_record::~boot_record(){
}

void boot_record::read_boot_record(FILE* file){
    fseek(file, 0, SEEK_SET);
    fread(this, sizeof(boot_record), 1, file);
}

void boot_record::print_partition_name(){
    cout << "Partition name: " << this->partition_name << endl;
}

void boot_record::print_bytes_per_sector(){
    cout << "Bytes per sector: " << this->bytes_per_sector << endl;
}

void boot_record::print_sectors_per_cluster(){
    cout << "Sectors per cluster: " << this->sectors_per_cluster << endl;
}

void boot_record::print_root_entry_count(){
    cout << "Root entry count: " << this->root_entry_count << endl;
}

void boot_record::print_bitmap_in_clusters(){
    cout << "Bitmap in clusters: " << this->bitmap_in_clusters << endl;
}

string boot_record::get_partition_name(){
    return this->partition_name;
}

int boot_record::get_bytes_per_sector(){
    return static_cast<int>(this->bytes_per_sector);
}

int boot_record::get_sectors_per_cluster(){
    return static_cast<int>(this->sectors_per_cluster);
}

int boot_record::get_root_entry_count(){
    return static_cast<int>(this->root_entry_count);
}

int boot_record::get_bitmap_in_clusters(){
    return static_cast<int>(this->bitmap_in_clusters);
}