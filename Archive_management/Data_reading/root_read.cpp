#include "root_directory.h"

root_directory::root_directory(/* args */)
{
}

root_directory::~root_directory()
{
}

root_directory::root_directory(FILE *partition)
{
    string filename;
    read_all_files(partition);
    cout << "Enter file name: " << endl;
    cin >> filename;
    int archive = search_data(partition, filename);
    read_data(partition, archive);
}

void root_directory::print_data_type()
{
    cout << "Data type: " << this->data_type << endl;
}

void root_directory::print_time_created()
{
    cout << "Time created: " << this->time_created << endl;
}

void root_directory::print_date_created()
{
    cout << "Date created: " << this->date_created << endl;
}

void root_directory::print_last_access()
{
    cout << "Last access: " << this->last_access << endl;
}

void root_directory::print_time_modified()
{
    cout << "Time modified: " << this->time_modified << endl;
}

void root_directory::print_date_modified()
{
    cout << "Date modified: " << this->date_modified << endl;
}

void root_directory::print_first_cluster()
{
    cout << "First cluster: " << this->first_cluster << endl;
}

void root_directory::print_file_size()
{
    cout << "File size: " << this->file_size << endl;
}

void root_directory::print_file_name()
{
    cout << "File name: " << this->file_name << endl;
}

int root_directory::get_data_type()
{
    return static_cast<int>(this->data_type);
}

int root_directory::get_time_created()
{
    return static_cast<int>(this->time_created);
}

int root_directory::get_date_created()
{
    return static_cast<int>(this->date_created);
}

int root_directory::get_last_access()
{
    return static_cast<int>(this->last_access);
}

int root_directory::get_time_modified()
{
    return static_cast<int>(this->time_modified);
}

int root_directory::get_date_modified()
{
    return static_cast<int>(this->date_modified);
}

int root_directory::get_first_cluster()
{
    return static_cast<int>(this->first_cluster);
}

int root_directory::get_file_size()
{
    return static_cast<int>(this->file_size);
}

string root_directory::get_file_name()
{
    return this->file_name;
}

int root_directory::search_data(FILE *partition, string name)
{
    int archive = 0;
    while (1)
    {
        fseek(partition, 513 + (sizeof(root_directory) * archive), SEEK_SET);
        fread(this, sizeof(root_directory), 1, partition);
        if (this->file_name == name)
        {
            return archive;
        }
        archive++;
    }
}

void root_directory::read_all_files(FILE *partition)
{
    boot_record boot;
    boot.read_boot_record(partition);
    int root_directories_entries = boot.get_root_entry_count();

    int currentEntryIndex = 0;
    cout << "Displaying all files" << endl;

    while (root_directories_entries >= 0)
    {
        fseek(partition, 513 + (sizeof(root_directory) * currentEntryIndex), SEEK_SET);
        fread(this, sizeof(root_directory), 1, partition);

        if (this->data_type == '0xFF')
        {
            string fileName = this->get_file_name();

            root_directory_map[currentEntryIndex] = fileName;
        }

        currentEntryIndex++;
        root_directories_entries--;
    }

    for (const auto &entry : root_directory_map)
    {
        int index = entry.first;
        const string &fileName = entry.second;

        cout << "Index: " << index << ", File Name: " << fileName << endl;
    }
}
void root_directory::print_archive_info()
{
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
void root_directory::read_data(FILE *partition, int archive)
{
    fseek(partition, 513 + (sizeof(root_directory) * archive), SEEK_SET);
    fread(this, sizeof(root_directory), 1, partition);
}

void root_directory::read_archive(FILE *partition)
{
    boot_record boot;
    int cluster_size = boot.get_sectors_per_cluster() * boot.get_bytes_per_sector();
    int first_cluster = this->get_first_cluster();
    fseek(partition, ((boot.get_root_entry_count() * 32) + 512) + (boot.get_bitmap_in_clusters() * cluster_size) + (first_cluster * (cluster_size)), SEEK_SET);
    char data[cluster_size - 4];
    int pointer;
    cout << " ---------------- DADOS ------------------" << endl;
    while (1)
    {
        fread(data, sizeof(cluster_size - 4), 1, partition);
        fread(&pointer, sizeof(4), 1, partition);
        for (int i = 0; i < this->get_file_size(); i++)
        {
            cout << data[i];
        }
        if (pointer == UINT_MAX)
        {
            break;
        }
        fseek(partition, ((boot.get_root_entry_count() * 32) + 512) + (boot.get_bitmap_in_clusters() * cluster_size) + (pointer * (cluster_size)), SEEK_SET);
    }
    cout << " ------------------------------------------------" << endl;
}