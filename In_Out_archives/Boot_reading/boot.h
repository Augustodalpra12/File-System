
class boot_record
{
private:
    char partition_name[8];
    short bytes_per_sector;
    char sectors_per_cluster;
    short root_entry_count;
    int bitmap_in_clusters;

public:
    boot_record();
    ~boot_record();
}__attribute__((packed));
