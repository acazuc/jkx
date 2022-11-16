#ifndef BUF_H
#define BUF_H

#include <stddef.h>
#include <stdint.h>

struct buf
{
	uint8_t *data;
	size_t capacity;
	size_t size;
	size_t pos;
};

void buf_init(struct buf *buf);
void buf_destroy(struct buf *buf);

int buf_reserve(struct buf *buf, size_t size);
size_t buf_remaining(struct buf *buf);

int buf_send(struct buf *buf, int fd);
int buf_recv(struct buf *buf, int fd);

int buf_wi8(struct buf *buf, int8_t v);
int buf_wu8(struct buf *buf, uint8_t v);
int buf_wi16(struct buf *buf, int16_t v);
int buf_wu16(struct buf *buf, uint16_t v);
int buf_wi32(struct buf *buf, int32_t v);
int buf_wu32(struct buf *buf, uint32_t v);
int buf_wstr(struct buf *buf, const char *v, size_t len);
int buf_wpad(struct buf *buf);

int buf_read(struct buf *buf, void *data, size_t size);
int buf_rrst(struct buf *buf);
int buf_ri8(struct buf *buf, int8_t *v);
int buf_ru8(struct buf *buf, uint8_t *v);
int buf_ri16(struct buf *buf, int16_t *v);
int buf_ru16(struct buf *buf, uint16_t *v);
int buf_ri32(struct buf *buf, int32_t *v);
int buf_ru32(struct buf *buf, uint32_t *v);
const char *buf_rstr(struct buf *buf);

#endif
