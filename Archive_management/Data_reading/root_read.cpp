#include "root_read.h"

root_read::root_read(/* args */)
{
}

root_read::~root_read()
{
}

root_read::root_read(FILE *partition)
{
    boot_record boot;
    boot.read_boot_record(partition);

    int index;
    read_all_files(partition);
    if (check_map_size())
    {
        return;
    }

    cout << "READING..." << endl;
    index = get_valid_index();
    search_data(partition, index);
    read_data(partition);
    update_metadata(index, partition);
    read_archive(partition);
}

int root_read::check_map_size()
{
    if (this->root_directory_map.size() == 0)
    {
        cout << "No files in the root directory" << endl;
        return 1;
    }
    return 0;
}
int root_read::is_index_valid(int index)
{

    for (const auto &entry : root_directory_map)
    {
        int aux_index = entry.first;
        if (aux_index == index && entry.second != "")
        {
            return 1;
        }
        cout << "Index: " << index << ", File Name: " << endl;
    }
    return 0;
}
int root_read::get_valid_index()
{
    int index;
    do
    {
        cout << "Enter Valid File Index " << endl;
        cin >> index;

    } while (!is_index_valid(index));
    return index;
}
void root_read::set_last_access(Date_Hour today, FILE *partition)
{
    fwrite(&today.date, sizeof(short), 1, partition);
}
void root_read::print_data_type()
{
    cout << "Data type: " << this->root.data_type << endl;
}

root_directory root_read::get_root()
{
    return this->root;
}

void root_read::print_time_created()
{
    cout << "Time created: " << this->root.time_created << endl;
}

void root_read::print_date_created()
{
    cout << "Date created: " << this->root.date_created << endl;
}

void root_read::print_last_access()
{
    cout << "Last access: " << this->root.last_access << endl;
}

void root_read::print_time_modified()
{
    cout << "Time modified: " << this->root.time_modified << endl;
}

void root_read::print_date_modified()
{
    cout << "Date modified: " << this->root.date_modified << endl;
}

void root_read::print_first_cluster()
{
    cout << "First cluster: " << this->root.first_cluster << endl;
}

void root_read::print_file_size()
{
    cout << "File size: " << this->root.file_size << endl;
}

void root_read::print_file_name()
{
    cout << "File name: " << this->root.file_name << endl;
}

int root_read::get_data_type()
{
    return static_cast<int>(this->root.data_type);
}

int root_read::get_time_created()
{
    return static_cast<int>(this->root.time_created);
}

int root_read::get_date_created()
{
    return static_cast<int>(this->root.date_created);
}

int root_read::get_last_access()
{
    return static_cast<int>(this->root.last_access);
}

int root_read::get_time_modified()
{
    return static_cast<int>(this->root.time_modified);
}

int root_read::get_date_modified()
{
    return static_cast<int>(this->root.date_modified);
}

int root_read::get_first_cluster()
{
    return static_cast<int>(this->root.first_cluster);
}

int root_read::get_file_size()
{
    return static_cast<int>(this->root.file_size);
}

string root_read::get_file_name()
{

    return this->root.file_name;
}

void root_read::search_data(FILE *partition, int index)
{
    boot_record boot;
    boot.read_boot_record(partition);
    int archive = 0;

    fseek(partition, boot.get_bytes_per_sector() + (32 * index), SEEK_SET);
}
void root_read::read_all_files(FILE *partition)
{
    boot_record boot;
    boot.read_boot_record(partition);
    int root_directories_entries = boot.get_root_entry_count();
    int currentEntryIndex = 0;
    cout << "Displaying all files" << endl;
    fseek(partition, boot.get_bytes_per_sector(), SEEK_SET);
    for (int i = 0; i < root_directories_entries; i++)
    {
        read_data(partition);
        int data_type = get_data_type();
        if (data_type == 18)
        {
            string fileName = get_file_name();
            this->root_directory_map[currentEntryIndex] = fileName;
        }

        currentEntryIndex++;
    }

    for (const auto &entry : root_directory_map)
    {
        int index = entry.first;
        const string &fileName = entry.second;

        cout << "Index: " << index << ", File Name: " << fileName << endl;
    }
}
void root_read::print_archive_info()
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
void root_read::read_data(FILE *partition)
{
    fread(&this->root.data_type, 1, 1, partition);
    fread(&this->root.time_created, 2, 1, partition);
    fread(&this->root.date_created, 2, 1, partition);
    fread(&this->root.last_access, 2, 1, partition);
    fread(&this->root.time_modified, 2, 1, partition);
    fread(&this->root.date_modified, 2, 1, partition);
    fread(&this->root.first_cluster, 4, 1, partition);
    fread(&this->root.file_size, 4, 1, partition);
    for (int i = 0; i < 13; i++)
    {
        fread(&this->root.file_name[i], 1, 1, partition);
    }
}

void root_read::update_metadata(int index, FILE *partition)
{
    boot_record boot;
    boot.read_boot_record(partition);
    fseek(partition, (boot.get_bytes_per_sector() + (32 * index)) + 5, SEEK_SET);
    tm *today = get_now();

    Date_Hour today_packed;

    pack packer(today->tm_year + 1900, today->tm_mon + 1, today->tm_mday, today->tm_hour, today->tm_min, today->tm_sec);

    today_packed.date = packer.pack_date();
    today_packed.time = packer.pack_time();
    set_last_access(today_packed, partition);
}

tm *root_read::get_now()
{
    time_t now = time(nullptr);
    tm *today = localtime(&now);

    return today;
}
void root_read::read_archive(FILE *partition)
{
    File_name file_name;
    boot_record boot;

    // file_name.set_name_to_new_file(this->get_file_name());
    // string name = file_name.get_name();
    // string extension = file_name.get_extension();
    string name;
    cout << "Input new file name: ";
    cin >> name;
    cout << endl;

    boot.read_boot_record(partition);
    int cluster_size = boot.get_sectors_per_cluster() * boot.get_bytes_per_sector();
    int first_cluster = this->get_first_cluster();

    char data[cluster_size - 4];
    int pointer = first_cluster;
    cout << " ---------------- DADOS ------------------" << endl;
    int bytes_read = 0;

    FILE *new_file;
    new_file = fopen(name.c_str(), "w+");

    while (bytes_read < this->get_file_size())
    {
        fseek(partition, (pointer * cluster_size), SEEK_SET);
        fread(&data, cluster_size - 4, 1, partition);
        bytes_read += cluster_size - 4;
        int write_size = cluster_size - 4;

        if (bytes_read > this->get_file_size())
            write_size -= (bytes_read - this->get_file_size());

        fread(&pointer, sizeof(4), 1, partition);
        for (int i = 0; i < write_size; i++)
        {
            fwrite(&data[i], 1, 1, new_file);
            printf("%c", data[i]);
        }
    }
    cout << endl;
    fclose(new_file);
    // while (1)
    // {
    //     fread(&data, cluster_size - 4, 1, partition);
    //     fread(&pointer, sizeof(4), 1, partition);
    //     for (int i = 0; i < this->get_file_size(); i++)
    //     {
    //         cout << data[i];
    //     }
    //     if (pointer == 0)
    //     {
    //         break;
    //     }
    //     fseek(partition, (pointer * (cluster_size)), SEEK_SET);
    // }
    cout << " ------------------------------------------------" << endl;
}