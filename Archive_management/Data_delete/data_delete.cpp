#include "data_delete.h"

data_delete::data_delete()
{
    // Implementação do construtor padrão (se necessário)
}

data_delete::data_delete(FILE *partition, boot_record boot)
{
    this->boot = boot;
}

data_delete::~data_delete()
{
}

void data_delete ::delete_file(FILE *partition)
{
    read_all_files(partition);
    cout << "Choose file to be deleted: ";
    // usuario vai escolher o int que vai ser deletado
    int archiveIndex;
    cin >> archiveIndex;
    cout << endl;
    get_file_clusters(partition);
    free_bitmap(partition);

    delete_metadata(partition, archiveIndex);
}

void data_delete ::get_file_clusters(FILE *partition)
{
    int cluster_size = this->boot.get_sectors_per_cluster() * this->boot.get_bytes_per_sector();
    int first_cluster = this->get_first_cluster();
    this->file_clusters.cluster_number = first_cluster;
    this->file_clusters.next = NULL;
    fseek(partition, (((this->boot.get_root_entry_count() * 32) + 512) + (this->boot.get_bitmap_in_clusters() * cluster_size) + (first_cluster * (cluster_size))) + 28, SEEK_SET);
    int pointer;
    while (pointer != UINT_MAX)
    {
        fread(&pointer, sizeof(int), 1, partition);
        this->file_clusters.next = new NODE;
        this->file_clusters.next->cluster_number = pointer;
        this->file_clusters.next->next = NULL;
        fseek(partition, ((this->boot.get_root_entry_count() * 32) + 512) + (this->boot.get_bitmap_in_clusters() * cluster_size) + (pointer * (cluster_size)), SEEK_SET);
    }
}

void data_delete ::free_bitmap(FILE *partition)
{
    NODE *cluster = &this->file_clusters;
    while (cluster != NULL)
    {
        flip_bit(0, cluster->cluster_number, partition);
        cluster = cluster->next;
    }
}

void data_delete::flip_bit(int mode, int position, FILE *partition)
{
    char bit;
    int root_in_sectors = ceil((this->boot.get_root_entry_count() * 32.0) / this->boot.get_bytes_per_sector());
    int root_and_boot_clusters = ceil((root_in_sectors + 1.0) / this->boot.get_sectors_per_cluster());

    int bitmap_start = this->boot.get_bytes_per_sector() * root_and_boot_clusters;
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

    fseek(partition, bitmap_start, SEEK_SET);
    fseek(partition, (position / 8), SEEK_CUR);

    char byte;

    fread(&byte, sizeof(byte), 1, partition);

    if (mode == 1)
        byte |= (bit);
    else
        byte &= ~(bit);

    fseek(partition, -1, SEEK_CUR);
    fwrite(&byte, sizeof(byte), 1, partition);
}

void data_delete::delete_metadata(FILE *partition, int archiveIndex)
{
    int root_start = (boot.get_sectors_per_cluster() * boot.get_bytes_per_sector());
    int file_position = root_start + (archiveIndex * 32);
    set_data_type(-1, partition);
}

void data_delete::set_data_type(char data_type, FILE *partition)
{
    fwrite(&data_type, sizeof(char), 1, partition);
}