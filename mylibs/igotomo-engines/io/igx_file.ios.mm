#include "igx_file.h"

const char* igx_file_writable_path()
{
    static char path[512];

    if (path[0]!=0)
    {
        return path;
    }

    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSLibraryDirectory, NSUserDomainMask, YES);
    NSString *documentsDirectory = [paths objectAtIndex:0];
    strcpy(path, [documentsDirectory UTF8String]);
    return path;
}
