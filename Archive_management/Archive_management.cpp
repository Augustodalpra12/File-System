#include ".\Archive_management.h"
/*g++ main.cpp .\Boot_reading\boot.cpp .\Data_reading\root_read.cpp .\Data_writing\data_write.cpp .\Data_delete\data_delete.cpp .\Assets\assets.cpp -o 
*/

Archive_management::Archive_management(FILE *partition)
{
    this->partition = partition;

    user_interface();
}

Archive_management::Archive_management(string filename)
{
    FILE *partition;
    partition = fopen(filename.c_str(), "r+b");

    this->partition = partition;

    user_interface();
}

void Archive_management::user_interface()
{
    int option = 0;
    boot_record boot;
    boot.read_boot_record(partition);

    data_delete data_deletar(partition, boot);

    while (true)
    {
        cout << endl << "File system menu" << endl;
        cout << "1. Write file" << endl;
        cout << "2. Delete file" << endl;
        cout << "3. Read file" << endl;
        cout << "4. Exit" << endl;
        cout << "Option: ";
        cin >> option;

        if (option == 1)
        {
            data_write data(boot, partition);

        } else if (option == 2) {
            data_deletar.delete_file(partition);

        } else if (option == 3) {
            root_read root(partition);
            
        } else if (option == 4) {
            exit(0);

        } else {

            cout << "Invalid option, try again" << endl;
        }
        
    }
}