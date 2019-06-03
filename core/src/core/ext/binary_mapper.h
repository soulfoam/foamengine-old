#pragma once

#include <cglm/cglm.h>
#include "sugar.h"

typedef enum
{
    BINARY_MAPPER_READ,
    BINARY_MAPPER_WRITE
} BinaryMapperMode;

typedef struct
{
    u8     *buffer;
    size_t buffer_size;
    size_t offset;
    size_t data_len;

    BinaryMapperMode mode;
} BinaryMapper;

BinaryMapper* binary_mapper_make(size_t buffer_size);
BinaryMapper* binary_mapper_load(const char *file_path);
BinaryMapper* binary_mapper_load_from_memory(const u8 *buffer, size_t buffer_size);
void          binary_mapper_destroy(BinaryMapper *mapper);
bool          binary_mapper_save(BinaryMapper *mapper, const char *file_path);
const void*   binary_mapper_buffer(BinaryMapper *mapper);
size_t        binary_mapper_buffer_size(BinaryMapper *mapper);
size_t        binary_mapper_offset(BinaryMapper *mapper);
void          bm_map_data(BinaryMapper *mapper, void *data, size_t data_len);
void          bm_map_char(BinaryMapper *mapper, char *str, size_t len);
void          bm_map_u8(BinaryMapper *mapper, u8 *data, size_t len);
void          bm_map_u16(BinaryMapper *mapper, u16 *data);
void          bm_map_u32(BinaryMapper *mapper, u32 *data);
void          bm_map_u64(BinaryMapper *mapper, u64 *data);
void          bm_map_s8(BinaryMapper *mapper, s8 *data, size_t len);
void          bm_map_s16(BinaryMapper *mapper, s16 *data);
void          bm_map_s32(BinaryMapper *mapper, s32 *data);
void          bm_map_f32(BinaryMapper *mapper, f32 *data);
void          bm_map_bool(BinaryMapper *mapper, bool *data);
void          bm_map_string(BinaryMapper *mapper, char *str, u32 str_len);
void          bm_map_vec2(BinaryMapper *mapper, vec2 v);
void          bm_map_vec3(BinaryMapper *mapper, vec3 v);
void          bm_map_vec4(BinaryMapper *mapper, vec4 v);
void          bm_map_mat4(BinaryMapper *mapper, mat4 m);
