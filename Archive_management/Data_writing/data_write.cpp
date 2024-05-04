#include "data_write.h"

data_write::data_write(boot_record boot, FILE* file)
{
    this->partition = file;
    this->boot = boot;
    this->file_clusters.cluster_number = -1;
    this->file_clusters.next = NULL;

    get_file();
    cout << "File name: " << this->file_name << endl;
    get_file_size();
    cout << "File size: " << this->file_size << endl;

    if (!check_available_clusters())
        return;
    
    set_occupied_bits();
    add_cluster(0); // Adding a final cluster pointing to 0, for us it means to nothing
    write_file();

    if (!write_on_root())
        return;
    

}

data_write::~data_write()
{

}

int data_write::get_file()
{
    get_filename();
    file_to_copy = fopen(file_name.c_str(), "r");
    if (file_to_copy == NULL)
    {
        cerr << "File not found" << endl;
        return 0;
    }

    while (file_name.size() > 10)
    {
        cout << "Enter a new name for the file ";
        cin >> this->file_name;
    }
    
    return 1;
}

void data_write::get_filename()
{
    cout << "Insert the name of the file to copy: ";
    cin >> file_name;
}

void data_write::get_file_size()
{
    fseek(this->file_to_copy, 0, SEEK_END);
    this->file_size = ftell(this->file_to_copy);
    fseek(this->file_to_copy, 0, SEEK_SET);
}

int data_write::check_available_clusters()
{
    int clusters_needed = ceil(this->file_size/((this->boot.get_bytes_per_sector()*this->boot.get_sectors_per_cluster()))); // Tamanho de cada cluster -4 bytes para endereçamento do próximo cluster

    int root_in_sectors = ceil((this->boot.get_root_entry_count() * 32.0) / this->boot.get_bytes_per_sector());
    int root_and_boot_clusters = ceil((root_in_sectors)/this->boot.get_sectors_per_cluster());
    
    bitmap_start = boot.get_bytes_per_sector() * root_and_boot_clusters;

    fseek(this->partition, bitmap_start, SEEK_SET);

    int clusters_found = 0;
    char byte;
    int bitmap_size = this->boot.get_bitmap_in_clusters() * (this->boot.get_sectors_per_cluster() * this->boot.get_bytes_per_sector());
    int i = 0;
    do
    {
        fread(&byte, sizeof(byte), 1, this->partition);
        if (byte != -1)
        {
            cout << "found space on the " << i+1 << " byte" << endl;
            clusters_found += count_free_bits(i, byte, clusters_needed-clusters_found);
        }

        i++;
    } while (clusters_found < clusters_needed && i < bitmap_size);

    if (clusters_found < clusters_needed)
    {
        cout << "Not enough space" << endl;
        return 0;
    }

    return 1;
}

int data_write::count_free_bits(int index, char byte, int spaces_needed)
{
    int count = 0; 

    bool bit[8];
    bit[7] = byte & 0b00000001;
    bit[6] = byte & 0b00000010;
    bit[5] = byte & 0b00000100;
    bit[4] = byte & 0b00001000;
    bit[3] = byte & 0b00010000;
    bit[2] = byte & 0b00100000;
    bit[1] = byte & 0b01000000;
    bit[0] = byte & 0b10000000;

    for (int i = 0; i < 8; i++)
    {                    
        if (!bit[i])
        {
            count++;
            add_cluster((index*8) + i);
        }

        if (count == spaces_needed)
            break;
    }
    
    return count;
}

int data_write::set_occupied_bits()
{
    NODE* cluster = &this->file_clusters;
    while (cluster != NULL)
    {
        flip_bit(1, cluster->cluster_number);
        cluster = cluster->next;
    }
    return 0;
}

void data_write::write_file()
{
    int cluster_size = (boot.get_sectors_per_cluster() * boot.get_bytes_per_sector()) - 4;
    char full_file[cluster_size];

    int bytes_read = 0;
    NODE* cluster = &this->file_clusters;

    while (bytes_read < this->file_size)
    {
        fread(&full_file, cluster_size, 1, file_to_copy);
        bytes_read += cluster_size;
        int write_size = cluster_size;

        if (bytes_read > this->file_size)
            write_size -= (bytes_read - this->file_size);

        int cluster_in_bytes = cluster->cluster_number * (cluster_size + 4);
        fseek(this->partition, cluster_in_bytes, SEEK_SET);
        fwrite(&full_file, write_size, 1, this->partition);

        NODE* next = cluster->next;

        fseek(this->partition, cluster_in_bytes+508, SEEK_SET);
        fwrite(&next->cluster_number, sizeof(int), 1, this->partition);

        cluster = cluster->next;
    }
}

int data_write::write_on_root()
{
    int root_start = (boot.get_sectors_per_cluster() * boot.get_bytes_per_sector());
    fseek(this->partition, root_start, SEEK_SET);

    cout << "Writing on root" << endl;

    if(!search_root_space())
    {
        cerr << "No space available on root" << endl;
        return 0;
    }

    set_data_type(19);

    tm* today = get_now();

    Date_Hour today_packed;

    today_packed.date = pack_date(today->tm_year + 1900, today->tm_mon + 1, today->tm_mday);
    today_packed.time = pack_time(today->tm_hour, today->tm_min, today->tm_sec);

    set_creation(today_packed);
    set_last_access(today_packed);
    set_modification(today_packed);

    set_first_cluster();
    set_file_size();
    set_filename();

    return 1;
}

int data_write::search_root_space()
{
    int root_entries = this->boot.get_root_entry_count();
    char type;
    for (int i = 0; i < root_entries; i++)
    {
        fread(&type, sizeof(char), 1, this->partition);
        cout << static_cast<int>(type) << endl;
        if(type == -1)
            return 1;
        fseek(this->partition, 31, SEEK_CUR);
    }
    return 0;
}

void data_write::set_data_type(char data_type)
{
    fwrite(&data_type, sizeof(char), 1, this->partition);
}

tm* data_write::get_now()
{
    time_t now = time(nullptr);
    tm* today = localtime(&now);

    return today;
}

void data_write::set_creation(Date_Hour today)
{
    fwrite(&today.time, sizeof(short), 1, this->partition);
    fwrite(&today.date, sizeof(short), 1, this->partition);
}

void data_write::set_last_access(Date_Hour today)
{
    fwrite(&today.date, sizeof(short), 1, this->partition);
}

void data_write::set_modification(Date_Hour today)
{
    fwrite(&today.time, sizeof(short), 1, this->partition);
    fwrite(&today.date, sizeof(short), 1, this->partition);
}

void data_write::set_first_cluster()
{
    int first_cluster = this->file_clusters.cluster_number;
    fwrite(&first_cluster, sizeof(int), 1, this->partition);
}

void data_write::set_file_size()
{
    fwrite(&this->file_size, sizeof(int), 1, this->partition);
}

void data_write::set_filename()
{
    fwrite(this->file_name.c_str(), sizeof(char), this->file_name.size(), this->partition);
}

void data_write::add_cluster(int cluster_number)
{
    if (this->file_clusters.cluster_number == -1)
    {
        this->file_clusters.cluster_number = cluster_number;    
        return;
    }

    NODE* aux = new NODE;
    NODE* end;
    aux->cluster_number = cluster_number;
    aux->next = NULL;

    end = &this->file_clusters;
    while (end->next != NULL)
    {
        end = end->next;
    }

    end->next = aux;
}

int data_write::flip_bit(int mode, int position)
{
    char bit;

    switch (position % 8)
    {
    case 0:
        bit = 128;
        break;

    case 1:
        bit = 64;
        break;

    case 2:
        bit = 32;
        break;

    case 3:
        bit = 16;
        break;

    case 4:
        bit = 8;
        break;

    case 5:
        bit = 4;
        break;

    case 6:
        bit = 2;
        break;
    case 7:
        bit = 1;
        break;
    
    default:
        break;
    }

    fseek(this->partition, bitmap_start, SEEK_SET);
    fseek(this->partition, (position/8), SEEK_CUR);

    char byte;

    fread(&byte, sizeof(byte), 1, this->partition);

    if (mode == 1)
        byte |= (bit);
    else
        byte &= ~(bit);

    fseek(this->partition, -1, SEEK_CUR);
    fwrite(&byte, sizeof(byte), 1, this->partition);

    return 1;
}

short data_write::pack_date(int year, int month, int day)
{
    short packed_date = 0;
    packed_date |= ((year - 2000) & 0x7F) << 9; // Representa 
    packed_date |= (month & 0xF) << 5;
    packed_date |= (day & 0x1F);

    return packed_date;
}

short data_write::pack_time(int hour, int minute, int second)
{
    short packed_hour = 0;
    packed_hour |= (hour & 0x1F) << 11;
    packed_hour |= (minute & 0x3F) << 5;

    int sec = second / 2;

    packed_hour |= (sec & 0x1F);

    return packed_hour;
}

int data_write::bitmap_position_to_cluster(int bit)
{

    return 0;
}

int data_write::cluster_to_bitmap_position(int cluster_number)
{

    return 0;
}