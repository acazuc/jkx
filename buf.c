#include "buf.h"
#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

void buf_init(struct buf *buf)
{
	buf->data = NULL;
	buf->capacity = 0;
	buf->size = 0;
}

void buf_destroy(struct buf *buf)
{
	free(buf->data);
}

int buf_reserve(struct buf *buf, size_t size)
{
	if (size <= buf->capacity)
		return 1;
	uint8_t *d = realloc(buf->data, size);
	if (!d)
		return 0;
	buf->data = d;
	buf->capacity = size;
	return 1;
}

size_t buf_remaining(struct buf *buf)
{
	if (buf->pos >= buf->size)
		return 0;
	return buf->size - buf->pos;
}

int buf_send(struct buf *buf, int fd)
{
	if (buf->pos >= buf->size)
		return 0;
	int ret = send(fd, &buf->data[buf->pos], buf->size - buf->pos, 0);
	if (ret < 0)
		return ret;
	buf->pos += ret;
	return ret;
}

int buf_recv(struct buf *buf, int fd)
{
	buf_rrst(buf);
	if (buf->size >= buf->capacity)
	{
		errno = EAGAIN;
		return -1;
	}
	int ret = recv(fd, &buf->data[buf->size], buf->capacity - buf->size, 0);
	if (ret < 0)
		return ret;
	buf->size += ret;
	return ret;
}

int buf_write(struct buf *buf, const void *data, size_t size)
{
	if (buf->size + size > buf->capacity)
	{
		uint8_t *d = realloc(buf->data, buf->size + size);
		if (!d)
			return 0;
		buf->data = d;
		buf->capacity = buf->size + size;
	}
	if (data)
		memcpy(&buf->data[buf->size], data, size);
	else
		memset(&buf->data[buf->size], 0, size);
	buf->size += size;
	return 1;
}

int buf_wi8(struct buf *buf, int8_t v)
{
	return buf_write(buf, &v, 1);
}

int buf_wu8(struct buf *buf, uint8_t v)
{
	return buf_write(buf, &v, 1);
}

int buf_wi16(struct buf *buf, int16_t v)
{
	return buf_write(buf, &v, 2);
}

int buf_wu16(struct buf *buf, uint16_t v)
{
	return buf_write(buf, &v, 2);
}

int buf_wi32(struct buf *buf, int32_t v)
{
	return buf_write(buf, &v, 4);
}

int buf_wu32(struct buf *buf, uint32_t v)
{
	return buf_write(buf, &v, 4);
}

int buf_wstr(struct buf *buf, const char *v, size_t len)
{
	return buf_write(buf, v, len);
}

int buf_wpad(struct buf *buf)
{
	static const char z[4] = {0};
	size_t m = buf->pos % 4;
	if (!m)
		return 1;
	return buf_write(buf, z, 4 - m);
}

int buf_rrst(struct buf *buf)
{
	memmove(buf->data, &buf->data[buf->pos], buf->size - buf->pos);
	buf->size -= buf->pos;
	buf->pos = 0;
}

int buf_read(struct buf *buf, void *data, size_t size)
{
	if (buf->pos + size > buf->size)
		return 0;
	if (data)
		memcpy(data, &buf->data[buf->pos], size);
	buf->pos += size;
	return 1;
}

int buf_ri8(struct buf *buf, int8_t *v)
{
	return buf_read(buf, v, 1);
}

int buf_ru8(struct buf *buf, uint8_t *v)
{
	return buf_read(buf, v, 1);
}

int buf_ri16(struct buf *buf, int16_t *v)
{
	return buf_read(buf, v, 2);
}

int buf_ru16(struct buf *buf, uint16_t *v)
{
	return buf_read(buf, v, 2);
}

int buf_ri32(struct buf *buf, int32_t *v)
{
	return buf_read(buf, v, 4);
}

int buf_ru32(struct buf *buf, uint32_t *v)
{
	return buf_read(buf, v, 4);
}

const char *buf_rstr(struct buf *buf)
{
	const char *base = (const char*)buf->data;
	while (buf->pos < buf->size)
	{
		buf->pos++;
		if (!buf->data[buf->pos - 1])
			return base;
	}
	return NULL;
}
