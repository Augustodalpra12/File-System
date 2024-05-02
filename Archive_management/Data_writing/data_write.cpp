#include "data_write.h"

data_write::data_write(boot_record boot, FILE* file)
{
    this->partition = file;
    get_file_size();
    this->boot = boot;
    
    if (!check_available_clusters())
        return;
    

}

data_write::~data_write()
{

}

void data_write::get_file_size()
{
    fseek(this->partition, 0, SEEK_END);
    this->file_size = ftell(this->partition);
    fseek(this->partition, 0, SEEK_SET);
}

int data_write::check_available_clusters()
{
    int clusters_needed = ceil(this->file_size/((this->boot.get_bytes_per_sector()*this->boot.get_sectors_per_cluster()) - 4.0)); // Tamanho de cada cluster -4 bytes para endereçamento do próximo cluster

    int root_in_sectors = ceil((this->boot.get_root_entry_count() * 32.0) / this->boot.get_bytes_per_sector());
    int root_and_boot_clusters = ceil(root_in_sectors + 1.0)/this->boot.get_sectors_per_cluster();
    
    bitmap_start = boot.get_bytes_per_sector() * root_and_boot_clusters;
    fseek(this->partition, bitmap_start, SEEK_SET);

    int clusters_found = 0;
    char byte;
    int bitmap_size = this->boot.get_bitmap_in_clusters() * (this->boot.get_sectors_per_cluster() * this->boot.get_bytes_per_sector());
    int i = 0;
    do
    {
        fread(&byte, sizeof(byte), 1, this->partition);
        if (byte != 255)
            clusters_found += count_free_bits(i, byte);
        
        i++;
    } while (i < bitmap_size && clusters_found < clusters_needed);

    if (clusters_found < clusters_needed)
    {
        cout << "Not enough space" << endl;
        return 0;
    }

    return 1;

}

int data_write::count_free_bits(int index, char byte)
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
        if (bit[i])
        {
            count++;
            add_cluster((index*8) + i);
        }
    }
    
    return count;
}

void data_write::add_cluster(int cluster_number)
{
    NODE aux;
    NODE* end;
    aux.cluster_number = cluster_number;
    aux.next = NULL;

    end = &this->file_clusters;
    while (end->next != NULL)
    {
        end = end->next;
    }
    end->next = &aux;

}

int data_write::set_occupied_bits()
{
    NODE* cluster = &this->file_clusters;
    while (cluster != NULL)
    {
        flip_bit(1, cluster->cluster_number);
        cluster = cluster->next;
    }
}

int data_write::bitmap_position_to_cluster(int bit)
{

}

int data_write::cluster_to_bitmap_position(int cluster_number)
{

}

int data_write::get_file()
{
    get_filename();
    file_to_copy = fopen(file_name.c_str(), "r");
}

void data_write::get_filename()
{
    cout << "Rename the file: ";
    cin >> file_name;
}

void data_write::set_data_type(int data_type)
{

}

void data_write::set_creation()
{

}

void data_write::set_last_access()
{

}

void data_write::set_modification()
{

}

void data_write::set_first_cluster()
{

}

void data_write::set_file_size()
{

}

void data_write::set_filename()
{

}

void data_write::write_file()
{
    string full_file;
    fread(&full_file, 512, 1, file_to_copy);

    cout << full_file << endl;
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