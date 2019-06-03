#pragma once

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <cglm/cglm.h>

#ifndef _WIN32
#include <arpa/inet.h>
#else
#include <winsock2.h>
#endif

static inline uint32_t 
io_get_file_size(const char *path)
{
	FILE *fp = fopen(path, "r");
	if (fp == NULL) printf("io_get_file_size failed for file %s\n", path);

	fseek(fp, 0L, SEEK_END);
	uint32_t ret = 0;
	ret = (uint32_t)ftell(fp);

	fclose(fp);

	return ret;
}

static inline uint32_t 
io_write_file(const char *path, const char *buffer, uint32_t len)
{
	FILE *fp = fopen(path, "w");
	if (fp == NULL)
	{
		printf("io_write_file failed for file %s\n", path);
		return 0;
	}

	fwrite(buffer, sizeof(char), len, fp);

	fclose(fp);

	return len;
}

static inline uint32_t 
io_read_file(const char *path, char **buffer)
{
	FILE *fp = fopen(path, "r");
	if (fp == NULL)
	{
		printf("io_read_file failed for file %s\n", path);
		return 0;
	}

	uint32_t file_size = io_get_file_size(path);

	*buffer = malloc(file_size);
	fread(*buffer, sizeof(char), file_size, fp);

	fclose(fp);

	return file_size;
}


static inline float 
htonf(float val) 
{
    uint32_t rep;
    memcpy(&rep, &val, sizeof rep);
    rep = htonl(rep);
    memcpy(&val, &rep, sizeof rep);
    return val;
}

static inline float 
ntohf(float val) 
{
    uint32_t rep;
    memcpy(&rep, &val, sizeof rep);
    rep = ntohl(rep);
    memcpy(&val, &rep, sizeof rep);
    return val;
}

static inline uint32_t 
io_write_char(char *buffer, const char *data, uint32_t len)
{
	memcpy(buffer, data, len);
	return len;
}

static inline uint32_t 
io_read_char(char *data, const char *buffer, uint32_t len)
{
	memcpy(data, buffer, len);
	return len;
}

static inline uint8_t 
io_write_uint8(char *buffer, uint8_t data)
{
	uint8_t len = sizeof(uint8_t);
	memcpy(buffer, &data, len);
	return len;
}

static inline uint8_t 
io_read_uint8(uint8_t *data, const char *buffer)
{
	uint8_t len = sizeof(uint8_t);
	memcpy(data, buffer, len);
	return len;
}

static inline uint8_t 
io_write_int8(char *buffer, int8_t data)
{
	uint8_t len = sizeof(int8_t);
	data = (int8_t)data;
	memcpy(buffer, &data, len);
	return len;
}

static inline uint8_t 
io_read_int8(int8_t *data, const char *buffer)
{
	uint8_t len = sizeof(int8_t);
	memcpy(data, buffer, len);
	return len;
}

static inline uint8_t 
io_write_uint16(char *buffer, uint16_t data)
{
	uint8_t len = sizeof(uint16_t);
	data = htons(data);
	memcpy(buffer, &data, len);
	return len;
}

static inline uint8_t 
io_read_uint16(uint16_t *data, const char *buffer)
{
	uint8_t len = sizeof(uint16_t);
	uint16_t data_ntohs;
	memcpy(&data_ntohs, buffer, len);
	*data = ntohs(data_ntohs);
	return len;
}

static inline uint8_t 
io_write_int16(char *buffer, int16_t data)
{
	uint8_t len = sizeof(uint16_t);
	data = htons((uint16_t)data);
	memcpy(buffer, &data, len);
	return len;
}

static inline uint8_t 
io_read_int16(int16_t *data, const char *buffer)
{
	uint8_t len = sizeof(uint16_t);
	uint16_t data_ntohs;
	memcpy(&data_ntohs, buffer, len);
	*data = ntohs((int16_t)data_ntohs);
	return len;
}

static inline uint8_t 
io_write_uint32(char *buffer, uint32_t data)
{
	uint8_t len = sizeof(uint32_t);
	data = htonl(data);
	memcpy(buffer, &data, len);
	return len;
}

static inline uint8_t 
io_read_uint32(uint32_t *data, const char *buffer)
{
	uint8_t len = sizeof(uint32_t);
	uint32_t data_ntohl;
	memcpy(&data_ntohl, buffer, len);
	*data = ntohl(data_ntohl);
	return len;
}

static inline uint8_t 
io_write_int32(char *buffer, int32_t data)
{
	uint8_t len = sizeof(uint32_t);
	data = htonl((uint32_t)data);
	memcpy(buffer, &data, len);
	return len;
}

static inline uint8_t 
io_read_int32(int32_t *data, const char *buffer)
{
	uint8_t len = sizeof(uint32_t);
	uint32_t data_ntohl;
	memcpy(&data_ntohl, buffer, len);
	*data = ntohl((int32_t)data_ntohl);
	return len;
}

static inline uint8_t 
io_write_float(char *buffer, float data)
{
	uint8_t len = sizeof(uint32_t);
	data = htonf(data);
	memcpy(buffer, &data, len);
	return len;
}

static inline uint8_t 
io_read_float(float *data, const char *buffer)
{
	uint8_t len = sizeof(uint32_t);
	float data_ntohl;
	memcpy(&data_ntohl, buffer, len);
	*data = ntohf(data_ntohl);
	return len;
}

static inline uint32_t 
io_write_vec2(char *buffer, vec2 v)
{
	uint32_t len = 0;

	for (uint8_t i = 0; i < 2; i++)
	{
		len += io_write_float(buffer + len, v[i]);
	}

	return len;
}

static inline uint32_t 
io_read_vec2(vec2 data, const char *buffer)
{
	uint32_t len = 0;

	for (uint8_t i = 0; i < 2; i++)
	{
		len += io_read_float(&data[i], buffer + len);
	}

	return len;
}

static inline uint32_t 
io_write_vec3(char *buffer, vec3 v)
{
	uint32_t len = 0;

	for (uint8_t i = 0; i < 3; i++)
	{
		len += io_write_float(buffer + len, v[i]);
	}

	return len;
}

static inline uint32_t 
io_read_vec3(vec3 data, const char *buffer)
{
	uint32_t len = 0;

	for (uint8_t i = 0; i < 3; i++)
	{
		len += io_read_float(&data[i], buffer + len);
	}
	
	return len;
}

static inline uint32_t 
io_write_vec4(char *buffer, vec4 v)
{
	uint32_t len = 0;

	for (uint8_t i = 0; i < 4; i++)
	{
		len += io_write_float(buffer + len, v[i]);
	}

	return len;
}

static inline uint32_t 
io_read_vec4(vec4 data, const char *buffer)
{
	uint32_t len = 0;

	for (uint8_t i = 0; i < 4; i++)
	{
		len += io_read_float(&data[i], buffer + len);
	}

	return len;
}

static inline uint32_t 
io_write_mat4(char *buffer, mat4 v)
{
	uint32_t len = 0;

	for (uint32_t col = 0; col < 4; col++)
	{
		for (uint32_t row = 0; row < 4; row++)
		{
			len += io_write_float(buffer + len, v[col][row]);
		}
	}

	return len;
}

static inline uint32_t 
io_read_mat4(mat4 data, const char *buffer)
{
	uint32_t len = 0;

	for (uint32_t col = 0; col < 4; col++)
	{
		for (uint32_t row = 0; row < 4; row++)
		{
			len += io_read_float(&data[col][row], buffer + len);
		}
	}

	return len;
}

