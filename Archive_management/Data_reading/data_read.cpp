#include "data_read.h"
#include <climits>

data_read::data_read(/* args */)
{
}

data_read::~data_read()
{
}

data_read::data_read(FILE* partition)
{
    string filename;
    cout << "Enter file name: " << endl;
    cin >> filename;
    int archive = search_data(partition, filename);
    read_data(partition, archive);
}

void data_read::print_data_type(){
    cout << "Data type: " << this->data_type << endl;
}

void data_read::print_time_created(){
    cout << "Time created: " << this->time_created << endl;
}

void data_read::print_date_created(){
    cout << "Date created: " << this->date_created << endl;
}

void data_read::print_last_access(){
    cout << "Last access: " << this->last_access << endl;
}

void data_read::print_time_modified(){
    cout << "Time modified: " << this->time_modified << endl;
}

void data_read::print_date_modified(){
    cout << "Date modified: " << this->date_modified << endl;
}

void data_read::print_first_cluster(){
    cout << "First cluster: " << this->first_cluster << endl;
}

void data_read::print_file_size(){
    cout << "File size: " << this->file_size << endl;
}

void data_read::print_file_name(){
    cout << "File name: " << this->file_name << endl;
}

int data_read::get_data_type(){
    return static_cast<int>(this->data_type);
}

int data_read::get_time_created(){
    return static_cast<int>(this->time_created);
}

int data_read::get_date_created(){
    return static_cast<int>(this->date_created);
}

int data_read::get_last_access(){
    return static_cast<int>(this->last_access);
}

int data_read::get_time_modified(){
    return static_cast<int>(this->time_modified);
}

int data_read::get_date_modified(){
    return static_cast<int>(this->date_modified);
}

int data_read::get_first_cluster(){
    return static_cast<int>(this->first_cluster);
}

int data_read::get_file_size(){
    return static_cast<int>(this->file_size);
}

string data_read::get_file_name(){
    return this->file_name;
}

int data_read::search_data(FILE* partition, string name){
    int archive = 0;
    while(1){
        fseek(partition, 513+(sizeof(data_read)*archive), SEEK_SET);
        fread(this, sizeof(data_read), 1, partition);
        if(this->file_name == name){
            return archive;
        }
        archive++;
    }
}

void data_read::read_all_files(FILE* partition){
    boot_record boot;
    boot.read_boot_record(partition);
    int root_directories_entries = boot.get_root_entry_count();
    while(root_directories_entries){ 
        fseek(partition, 513+(sizeof(data_read)*root_directories_entries), SEEK_SET);
        fread(this, sizeof(data_read), 1, partition);
        if(this->data_type == '0xFF') {
            print_archive_info();
        }

        root_directories_entries--;
    }
}
void data_read::print_archive_info(){
    cout << "----------------------------------------------------------" << endl;
    print_data_type();
    print_time_created();
    print_date_created();
    print_last_access();
    print_time_modified();
    print_date_modified();
    print_first_cluster();
    print_file_size();
    print_file_name();
}
void data_read::read_data(FILE* partition, int archive){
    fseek(partition, 513+(sizeof(data_read)*archive), SEEK_SET);
    fread(this, sizeof(data_read), 1, partition);
}

void data_read::read_archive(FILE* partition){
    boot_record boot;
    int cluster_size = boot.get_sectors_per_cluster()*boot.get_bytes_per_sector();
    int first_cluster = this->get_first_cluster();
    fseek(partition, ((boot.get_root_entry_count()*32)+512)+(boot.get_bitmap_in_clusters()*cluster_size)+(first_cluster*(cluster_size)), SEEK_SET);
    char data[cluster_size-4];
    int pointer;
    cout << " ---------------- DADOS ------------------" << endl;
    while(1){
        fread(data, sizeof(cluster_size-4), 1, partition);
        fread(&pointer, sizeof(4), 1, partition);
        for(int i = 0; i < this->get_file_size(); i++){
            cout << data[i];
        }
        if(pointer == UINT_MAX){
            break;
        }
        fseek(partition, ((boot.get_root_entry_count()*32)+512)+(boot.get_bitmap_in_clusters()*cluster_size)+(pointer*(cluster_size)), SEEK_SET);

    }
    cout << " ------------------------------------------------" << endl;
}