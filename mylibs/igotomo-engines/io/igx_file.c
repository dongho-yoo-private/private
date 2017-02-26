#include "igx_file.h"
#include <stdlib.h>
#include <string.h>
#include "library/igx_memory.h"
#include "debug/igx_debug.h"

/* for mmap */
#if OS_IS_LINUX==1
#   include <sys/mman.h>
#   include <stdio.h>
#   include <fcntl.h>
#endif
#if OS_IS_MACOSX==1
#   include <sys/types.h>
#   include <sys/mman.h>
#endif
#if OS_IS_IOS==1
#   include <sys/types.h>
#   include <sys/mman.h>
#endif



#define __BUFFERSIZE_PER_ONCE__ (8192)

#ifdef __POSIX__
typedef int igx_file_t;
#endif
#ifdef WIN32
typedef HANDLE igx_file_t;
#endif

static ubit32_t igx_file_last_error=0;


#ifdef __POSIX__
#define IGX_FILE_MALLOC(a) IGX_MALLOC(a, "igx_flile")
#define IGX_FILE_FREE IGX_FREE

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

bit32_t igx_file_read(const char* file_name, void* buffer, int index, size_t size)
{
	int sum=0;
	igx_file_t fd = open(file_name, O_RDONLY, kFileMode666);
	char* p_buffer;
	
	if (fd<0)
	{
        // open error!
        igx_file_last_error=1;
		return 0;
	}

	lseek(fd, index, SEEK_SET);	

	p_buffer = (char*)buffer;
	while(size)
	{
		int read_once = (__BUFFERSIZE_PER_ONCE__ < size) ? __BUFFERSIZE_PER_ONCE__:size;
		int res = read(fd, &p_buffer[sum], read_once); 

		if (res<0)
		{
            // read error!
            igx_file_last_error=2;
			return 0;
		}
		
		size -= res;
		sum+=res;
	}
	
	close(fd);

    igx_file_last_error=0;
	return sum;
}
bit32_t igx_file_write(const char* file_name, const void* buffer, int index, size_t size)
{
    size_t sum=0;
	int fd = open(file_name, O_WRONLY|O_CREAT|O_TRUNC, kFileMode666); 

	if (fd<0)
	{
        igx_file_last_error=1;
		return 0;
	}

	if (flock(fd, LOCK_EX)!=0)
	{
        // lock error
        igx_file_last_error=3;
		close(fd);
		return 0;
	}

	lseek(fd, index, SEEK_SET);

	while(size)
	{
		size_t write_once = __BUFFERSIZE_PER_ONCE__ < size ? __BUFFERSIZE_PER_ONCE__:size;
        size_t n;
        if ((n=write(fd, buffer, write_once))!=write_once)
        {
            // write error
            igx_file_last_error=4;
		    close(fd);
            IGX_CRITICAL("write error! (%s)", file_name);
		    return 0;
        }
		size -= n;
        sum+=n;
	}

	if (flock(fd, LOCK_UN)!=0)
	{
        // unlock error
        close(fd);
        igx_file_last_error=5;
            IGX_CRITICAL("unlock error!! (%s)", file_name);
		return 0;
	}
	
	close(fd);

    igx_file_last_error=0;
	return (bit32_t)sum;
}
bit32_t igx_file_append(const char* file_name, void* buffer, size_t size)
{
	int fd = open(file_name, O_APPEND|O_CREAT, kFileMode666); 

	if (fd<0)
	{
        igx_file_last_error=1;
		return 0;
	}

	if (flock(fd, LOCK_EX)!=0)
	{
        igx_file_last_error=3;
		close(fd);
		return 0;
	}

	while(size)
	{
		int write_once = __BUFFERSIZE_PER_ONCE__ < size ? __BUFFERSIZE_PER_ONCE__:size;
        if (write(fd, buffer, write_once)!=write_once)
        {
            igx_file_last_error=4;
		    close(fd);
		    return 0;
        }
		size -= write(fd, buffer, write_once);
	}

	if (flock(fd, LOCK_UN)!=0)
	{
        igx_file_last_error=5;
		return 0;
	}
	
	close(fd);

    igx_file_last_error=0;
	return size;
}
void* igx_file_load(const char* file_name, void* dest, size_t* size, igx_file_security_t* secur)
{
	ubit8_t* p_dest;
    bool_t dest_is_allocated=false;
 	bit32_t sum=0, file_size=0;
    struct stat file_stat={0, };
    const size_t read_once_size = __BUFFERSIZE_PER_ONCE__;
	igx_file_t fd = open(file_name, O_RDONLY, kFileMode666);

	if (fd<0)
	{
        // open error!
        igx_file_last_error=1;
        IGX_ERROR("open error! %s err:%d", file_name, errno);
		return 0;
	}
    if (fstat(fd, &file_stat)==-1)
    {
        igx_file_last_error=5;
        IGX_ERROR("fstat error! err:%d", errno);
		return 0;
    }
    *size = file_size = file_stat.st_size;

    if (dest==0)
    {
        dest = IGX_FILE_MALLOC(file_size+1);
        dest_is_allocated=true;
    }
    p_dest = (ubit8_t*)dest;
    p_dest[file_size]=0;
	
	lseek(fd, 0, SEEK_SET);	
	while(file_size)
	{
		size_t read_once = (read_once_size<*size)?read_once_size:*size;
		int    res = read(fd, p_dest+sum, read_once); 

		if (res<0)
		{
            // read error!
            IGX_ERROR("read error ! (%d) err:%d", res, errno);
            igx_file_last_error=2;
            *size=0;
			return 0;
		}
		
		file_size -= res;
		sum+=res;
	}
	
	close(fd);

    if (secur!=0)
    {
        ubit8_t* ret = (ubit8_t*)IGX_FILE_MALLOC(*size+1);
        IGX_INFO("load secure mode!");
        ret[*size]=0;
        ret = (ubit8_t*)secur->decrypt_func(ret, size, dest, *size, secur->key);
        if (dest_is_allocated==true)
        {
            IGX_FILE_FREE(dest);
        }

        igx_file_last_error=(ret!=0)?0:6;
        return ret;
    }
    igx_file_last_error=0;

    return dest;
}
void* igx_file_vload(const char* file_name, size_t* size, igx_file_security_t* secur)
{
	ubit8_t* p_dest;
 	bit32_t sum=0, file_size=0;
    struct stat file_stat={0, };
    const size_t read_once_size = __BUFFERSIZE_PER_ONCE__;
	igx_file_t fd = open(file_name, O_RDONLY, kFileMode666);

	if (fd<0)
	{
        // open error!
        igx_file_last_error=1;
        IGX_ERROR("open error! %s err:%d", file_name, errno);
		return 0;
	}
    if (fstat(fd, &file_stat)==-1)
    {
        igx_file_last_error=5;
        IGX_ERROR("fstat error! err:%d", errno);
		return 0;
    }
    *size = file_size = file_stat.st_size;

    p_dest = (ubit8_t*)mmap(0, file_size+1, PROT_READ|PROT_WRITE, MAP_PRIVATE, fd, 0);
    if(p_dest == MAP_FAILED)
    {
        IGX_ERROR("Error: mmap() %s\n", strerror(errno)); 
        close(fd);
        return 0;
    }
    p_dest[file_size]=0;
	
	lseek(fd, 0, SEEK_SET);	
	while(file_size)
	{
		size_t read_once = (read_once_size<*size)?read_once_size:*size;
		int    res = read(fd, p_dest+sum, read_once); 

		if (res<0)
		{
            // read error!
            IGX_ERROR("read error ! (%d) err:%d", res, errno);
            igx_file_last_error=2;
            *size=0;
			return 0;
		}
		
		file_size -= res;
		sum+=res;
	}
	
	close(fd);

    if (secur!=0)
    {
        ubit8_t* ret = (ubit8_t*)igx_vmalloc(*size+1);
        IGX_INFO("load secure mode!(%s)", file_name);
        ret[*size]=0;
        ret = (ubit8_t*)secur->decrypt_func(ret, size, p_dest, *size, secur->key);
        igx_file_unvload(p_dest);

        igx_file_last_error=(ret!=0)?0:6;
        return ret;
    }
    igx_file_last_error=0;

    return p_dest;
}
size_t igx_file_write_s(const char* file_name, const void* buffer, size_t size, igx_file_security_t* secur)
{
    if (secur!=0)
    {
        size_t ret;
        size_t new_size = igx_encrypted_size(size);
        void* encrypt_buffer = IGX_FILE_MALLOC(new_size);
        secur->encrypt_func(encrypt_buffer, &new_size, buffer, size, secur->key);
        ret = igx_file_write(file_name, encrypt_buffer, 0, size);
        IGX_FILE_FREE(encrypt_buffer); 
        return ret;
    }
    return igx_file_write(file_name, buffer, 0, size);
}
void igx_get_default_security(igx_file_security_t* security, ubit32_t key)
{
    security->encrypt_func = igx_encrypt;
    security->decrypt_func = igx_decrypt;
    security->key     = key;
}
bool_t igx_file_mkdir(const char* path, igx_file_mode_t mode)
{
    return mkdir(path, mode)==0;
}
bool_t igx_file_copy(const char* dest_path, const char* src_path)
{
    size_t size;
    void* p = igx_file_load(src_path, 0, &size, 0); 
    if (p==0)
    {
        return false;
    }
    if (igx_file_write(dest_path, p, 0, size)==size)
    {
        igx_file_unload(p);
        return true; 
    }

    igx_file_unload(p);
    return false;
}
bool_t igx_file_move(const char* dest_path, const char* src_path)
{
    return rename(src_path, dest_path)==0;
}
bool_t igx_file_remove(const char* path)
{
    return remove(path)==0;
}
size_t igx_file_size(const char* filename)
{
    struct stat file_stat = {0, };
    if (stat(filename, &file_stat)==-1)
    {
        igx_file_last_error=5;
		return 0;
    }
    return file_stat.st_size;
}
#else
#   include "igx_file.win32.c"
#endif

void igx_file_unload(void*p)
{
    IGX_FILE_FREE(p);
}
void igx_file_unvload(void*p)
{
    igx_vfree(p);
}
const char* igx_file_error_msg[] =
{
    "success!",
    "open error!",
    "read error!",
    "lock error!",
    "write error!",
    "unlock error!",
    "decrypt error!"
};
const char* igx_file_get_last_error(ubit32_t* error_no)
{
    if (error_no)
    {
        *error_no = igx_file_last_error;
    }
    return igx_file_error_msg[igx_file_last_error];
}
char* igx_get_string_from_io(int from_fd, char* buffer, size_t buffer_size, size_t* size, char break_ch)
{
#define kREAD_ONCE_BUFFER_SIZE (4096)
    int sum=0;
    size_t length;
    char temp[kREAD_ONCE_BUFFER_SIZE+1];

    while(1)
    {
        length = read(from_fd, temp, kREAD_ONCE_BUFFER_SIZE);

        if ((sum+length)>=buffer_size)
        {
            buffer[sum]=0;
            break;
        }
        if (length==0)
        {
            buffer[sum]=0;
            break;
        }
        memcpy(buffer+sum, temp, length);
        sum+=length;
        if (temp[length-1]==break_ch)
        {
            buffer[sum]=0;
            break;
        }
 
    }
    if (size!=0)
    {
        *size = sum;
    }
    return buffer;
}

#if OS_IS_MACOSX == 1
const char* igx_file_writable_path()
{
    return "./";
}
#endif


