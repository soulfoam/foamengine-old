#ifndef _WIN32
#include <arpa/inet.h>
#else
#include <winsock2.h>
#endif

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <cglm/cglm.h>

#include "binary_mapper.h"

BinaryMapper*
binary_mapper_make(size_t buffer_size)
{
    BinaryMapper *mapper = malloc(sizeof(*mapper));
    mapper->buffer       = malloc(buffer_size);
    mapper->buffer_size  = buffer_size;
    mapper->offset       = 0;
    mapper->data_len     = 0;
    mapper->mode         = BINARY_MAPPER_WRITE;

    return mapper;
}

BinaryMapper*
binary_mapper_load(const char *file_path)
{
    FILE *fp = fopen(file_path, "rb");
    if (fp == NULL)
    {
        printf("Binary mapper was unable to open file %s\n", file_path);
        return NULL;
    }

    fseek(fp, 0, SEEK_END);
    size_t file_size = ftell(fp);
    rewind(fp);

    BinaryMapper *mapper = binary_mapper_make(file_size);
    mapper->mode         = BINARY_MAPPER_READ;
    mapper->data_len     = file_size;
    fread(mapper->buffer, sizeof(u8), file_size, fp);

    fclose(fp);

    return mapper;
}

BinaryMapper*
binary_mapper_load_from_memory(const u8 *buffer, size_t buffer_size)
{
    BinaryMapper *mapper = binary_mapper_make(buffer_size);
    mapper->mode         = BINARY_MAPPER_READ;
    memcpy(mapper->buffer, buffer, buffer_size);

    return mapper;
}

void
binary_mapper_destroy(BinaryMapper *mapper)
{
    free(mapper->buffer);
    free(mapper);
}

bool
binary_mapper_save(BinaryMapper *mapper, const char *file_path)
{
    FILE *fp = fopen(file_path, "wb");
    if (fp == NULL)
    {
        printf("Binary mapper was unable to save file %s\n", file_path);
        return false;
    }

    fwrite(mapper->buffer, sizeof(u8), mapper->data_len, fp);

    fclose(fp);

    return true;
}

const void*
binary_mapper_buffer(BinaryMapper *mapper)
{
    return mapper->buffer;
}

size_t      
binary_mapper_buffer_size(BinaryMapper *mapper)
{
    return mapper->buffer_size;
}

size_t      
binary_mapper_offset(BinaryMapper *mapper)
{
    return mapper->offset;
}

void
bm_map_data(BinaryMapper *mapper, void *data, size_t data_len)
{
    if (mapper->mode == BINARY_MAPPER_READ)
    {
        memcpy(data, mapper->buffer + mapper->offset, data_len);
    }
    else if (mapper->mode == BINARY_MAPPER_WRITE)
    {
        memcpy(mapper->buffer + mapper->offset, data, data_len);
    }

    mapper->offset   += data_len;
    mapper->data_len += data_len;
}

void        
bm_map_char(BinaryMapper *mapper, char *buf, size_t len)
{
    bm_map_data(mapper, buf, len);
}

void        
bm_map_u8(BinaryMapper *mapper, u8 *data, size_t len)
{
    bm_map_data(mapper, data, len);
}

void        
bm_map_u16(BinaryMapper *mapper, u16 *data)
{
    u8 bytes[sizeof(u16)];

    if (mapper->mode == BINARY_MAPPER_READ) 
    {
        bm_map_data(mapper, bytes, sizeof(u16));
        *data = ((bytes[1] << 0) | 
                (bytes[0] << 8)) ;
    }
    else if (mapper->mode == BINARY_MAPPER_WRITE)
    {
        bytes[0] = (*data >> 8) & 0xFF;
        bytes[1] = (*data >> 0) & 0xFF;
        bm_map_data(mapper, bytes, sizeof(u16));
    }
}

void        
bm_map_u32(BinaryMapper *mapper, u32 *data)
{
    u8 bytes[sizeof(u32)];

    if (mapper->mode == BINARY_MAPPER_READ) 
    {
        bm_map_data(mapper, bytes, sizeof(u32));  

        *data = ((u32)bytes[3] << 0)  | 
                ((u32)bytes[2] << 8)  | 
                ((u32)bytes[1] << 16) | 
                ((u32)bytes[0] << 24) ;
    }
    else if (mapper->mode == BINARY_MAPPER_WRITE)
    {
        bytes[0] = (*data >> 24) & 0xFF;
        bytes[1] = (*data >> 16) & 0xFF;
        bytes[2] = (*data >>  8) & 0xFF;
        bytes[3] = (*data >>  0) & 0xFF;

        bm_map_data(mapper, bytes, sizeof(u32));
    }
}

void        
bm_map_u64(BinaryMapper *mapper, u64 *data)
{
    u8 bytes[sizeof(u64)];

    if (mapper->mode == BINARY_MAPPER_READ) 
    {
        bm_map_data(mapper, bytes, sizeof(u64));  

        *data = ((u64)bytes[7] << 0)  |
                ((u64)bytes[6] << 8)  |
                ((u64)bytes[5] << 16) |
                ((u64)bytes[4] << 24) |
                ((u64)bytes[3] << 32) |
                ((u64)bytes[2] << 40) |
                ((u64)bytes[1] << 48) |
                ((u64)bytes[0] << 56) ;
    }
    else if (mapper->mode == BINARY_MAPPER_WRITE)
    {
        bytes[0] = (*data >> 56) & 0xFF;
        bytes[1] = (*data >> 48) & 0xFF;
        bytes[2] = (*data >> 40) & 0xFF;
        bytes[3] = (*data >> 32) & 0xFF;
        bytes[4] = (*data >> 24) & 0xFF;
        bytes[5] = (*data >> 16) & 0xFF;
        bytes[6] = (*data >>  8) & 0xFF;
        bytes[7] = (*data >>  0) & 0xFF;

        bm_map_data(mapper, bytes, sizeof(u64));
    }
}

void        
bm_map_s8(BinaryMapper *mapper, s8 *data, size_t len)
{
    bm_map_data(mapper, data, len);
}

void        
bm_map_s16(BinaryMapper *mapper, s16 *data)
{
    bm_map_u16(mapper, (u16*) data);
}

void        
bm_map_s32(BinaryMapper *mapper, s32 *data)
{
    bm_map_u32(mapper, (u32*) data);
}

void        
bm_map_f32(BinaryMapper *mapper, f32 *data)
{
    bm_map_u32(mapper, (u32*) data);
}

void        
bm_map_bool(BinaryMapper *mapper, bool *data)
{
    bm_map_data(mapper, data, sizeof(bool));
}

void        
bm_map_string(BinaryMapper *mapper, char *str, u32 str_len)
{
    bm_map_u32(mapper, &str_len);
    bm_map_data(mapper, str, str_len);
}

void        
bm_map_vec2(BinaryMapper *mapper, vec2 v)
{
    bm_map_f32(mapper, &v[0]);
    bm_map_f32(mapper, &v[1]);
}

void        
bm_map_vec3(BinaryMapper *mapper, vec3 v)
{
    bm_map_f32(mapper, &v[0]);
    bm_map_f32(mapper, &v[1]);
    bm_map_f32(mapper, &v[2]);
}

void        
bm_map_vec4(BinaryMapper *mapper, vec4 v)
{
    bm_map_f32(mapper, &v[0]);
    bm_map_f32(mapper, &v[1]);
    bm_map_f32(mapper, &v[2]);
    bm_map_f32(mapper, &v[3]);
}

void        
bm_map_mat4(BinaryMapper *mapper, mat4 m)
{
    for (u32 col = 0; col < 4; col++)
    {
        for (u32 row = 0; row < 4; row++)
        {
            bm_map_f32(mapper, &m[col][row]);
        }
    }
}

