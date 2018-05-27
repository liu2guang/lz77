#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "rtthread.h"
#include "lz77.h" 
#include "finsh.h"

#define LZ77_PRINT(fmt, ...)              \
do{                                       \
    rt_kprintf(fmt, ##__VA_ARGS__);       \
}while(0)

#define LZ77_INFO(fmt, ...)               \
do{                                       \
    rt_kprintf("[\033[32mLZ77\033[0m] "); \
    rt_kprintf(fmt, ##__VA_ARGS__);       \
}while(0)

/* 加密窗口 */
#define MIN_LA_SIZE 2 
#define MAX_LA_SIZE 255 

/* 搜索窗口 */ 
#define MIN_SB_SIZE 1 
#define MAX_SB_SIZE 65535 

/* 窗口大小: -1默认la=15, sb=4096 */ 
int la_size = -1; 
int sb_size = -1;

int lz77(int argc, char *argv[]) 
{
    FILE *file = RT_NULL;
    struct bitFILE *bitF = RT_NULL; 
    
    const char *help_info[] = 
    {
        [0] = "  -m     run mode, lz77 -m encode/decode <filename_in> <filename_out>.", 
        [1] = "  -la    lookahead size(def 15), lz77 -la <2~255>.", 
        [2] = "  -sb    search buf size(def 4096), lz77 -sb <1~65535>.",
    }; 
    
    if(argc < 2)
    {
        uint8_t index = 0;
        LZ77_PRINT("\033[32mUsage: lz77 [-m] [-la] [-sb] \033[0m\n"); 
        LZ77_PRINT("optional arguments: \n"); 
        for(index = 0; index < (sizeof(help_info)/sizeof(char*)); index++)
        {
            LZ77_PRINT("%s\n", help_info[index]);
        }
        return RT_EOK; 
    }
    else
    {
        if(!strcmp(argv[1], "-m")) 
        {
            if((!strcmp(argv[2], "e")) || (!strcmp(argv[2], "encode")))
            {
                if((file = fopen(argv[3], "rb")) == NULL)
                {
                    LZ77_INFO("open input file failed.\n");
                    goto failed; 
                }
                if ((bitF = bitIO_open(argv[4], BIT_IO_W)) == NULL)
                {
                    LZ77_INFO("open output file failed.\n");
                    goto failed; 
                }
                encode(file, bitF, la_size, sb_size); 
            }
            else if((!strcmp(argv[2], "d")) || (!strcmp(argv[2], "decode")))
            {
                if((bitF = bitIO_open(argv[3], BIT_IO_R)) == NULL)
                {
                    LZ77_INFO("open input file failed.\n");
                    goto failed; 
                }
                if((file = fopen(argv[4], "w")) == NULL)
                {
                    LZ77_INFO("open output file failed.\n");
                    goto failed; 
                }
                decode(bitF, file);
            }
            else
            {
                LZ77_PRINT("\033[31mError format: lz77 -m encode/decode <filename_in> <filename_out>. \033[0m\n"); 
                return RT_ERROR; 
            }
        }
        else if(!strcmp(argv[1], "-la"))
        {
            la_size = atoi(argv[2]);
            if(la_size < MIN_LA_SIZE || la_size > MAX_LA_SIZE)
            {
                LZ77_INFO("set lookahead size failed range 2~255.\n"); 
                goto failed; 
            }
        }
        else if(!strcmp(argv[1], "-sb"))
        {
            sb_size = atoi(argv[2]); 
            if(sb_size < MIN_SB_SIZE || la_size > MAX_SB_SIZE)
            {
                LZ77_INFO("set lookahead size failed range 1~65535.\n"); 
                goto failed; 
            }
        }
        else
        {
            LZ77_PRINT("\033[32mUsage: lz77 [-m] [-la] [sb] \033[0m\n");
            LZ77_PRINT("Error arguments: \033[31m%s\033[0m. \n", argv[1]); 
            goto failed; 
        }
    }
    
    fclose(file);
    bitIO_close(bitF);
    return RT_EOK; 

failed: 
    if(file != RT_NULL)
    {
        fclose(file); 
        file = RT_NULL; 
    }
    if(bitF != RT_NULL)
    {
        bitIO_close(bitF); 
        file = RT_NULL; 
    }
    return RT_ERROR; 
}
MSH_CMD_EXPORT(lz77, Lempel Ziv Encoding and Decoding); 
