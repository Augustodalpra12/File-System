#include "./Boot_reading/boot.h"
#include "./Data_writing/data_write.h"
#include "./Data_delete/data_delete.h"

class Archive_management
{
private:
    FILE *partition;
public:
    Archive_management(FILE *partition);
    Archive_management(string filename);

    void user_interface();
};