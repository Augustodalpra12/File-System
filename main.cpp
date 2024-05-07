#include "./Format/formater.h"
#include "./Archive_management/Archive_management.h"

int main(int argc, char const *argv[])
{
    cout << "Want to open an image file or create a new one? new/open " << endl;
    string response;

    cin >> response;

    while (response != "new" && response != "open")
    {
        cout << "Invalid option, try again" << endl;
        cin >> response;
    }
    
    if (response == "new")
    {
        formater new_disk;
        string filename = new_disk.get_filename();
        Archive_management file_system(filename);
    }
    if (response == "open")
    {
        cout << "Enter file name with extension: ";
        string filename;
        cin >> filename;

        FILE *partition;

        partition = fopen(filename.c_str(), "r+b");
        while (partition == NULL)
        {
            cout << "File not found. Enter a valid file name" << endl;
            cin >> filename;
            partition = fopen(filename.c_str(), "r+b");
        }

        Archive_management file_systems(partition);
    }
    
    return 0;
}

