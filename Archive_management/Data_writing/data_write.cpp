#include "data_write.h"

data_write::data_write(boot_record boot, FILE* file)
{
    this->file = file;
    get_file_size();
    this->boot = boot;
    
    check_available_clusters();
    
}

data_write::~data_write()
{

}

void data_write::get_file_size()
{
    fseek(this->file, 0, SEEK_END);
    this->file_size = ftell(this->file);
    fseek(this->file, 0, SEEK_SET);
}

int data_write::check_available_clusters()
{
    int clusters_needed = ceil(this->file_size/((this->boot.get_bytes_per_sector()*this->boot.get_sectors_per_cluster()) - 4.0)); // Tamanho de cada cluster -4 bytes para endereçamento do próximo cluster

    int root_in_sectors = ceil((this->boot.get_root_entry_count() * 32.0) / this->boot.get_bytes_per_sector());
    int root_and_boot_clusters = ceil(root_in_sectors + 1.0)/this->boot.get_sectors_per_cluster();
    int bitmap_start = boot.get_bytes_per_sector() * root_and_boot_clusters;
    fseek(this->file, bitmap_start, SEEK_SET);

    int clusters_found = 0;
    char byte;
    int i = 0;
    while(clusters_found < clusters_needed)
    {
        fread(&byte, sizeof(byte), 1, this->file);
        if (byte != 255)
        {
            clusters_found = count_free_bits(i, byte);
        }
        i++;
    }
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

NODE* data_write::serach_neadeed_clusters()
{

}

int data_write::set_occupied_bits()
{

}

int data_write::bitmap_position_to_cluster(int bit)
{

}

int data_write::cluster_to_bitmap_position(int cluster_number)
{

}

int data_write::get_file()
{

}

void data_write::get_filename()
{

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

int data_write::flip_bit(int mode, int position)
{

}