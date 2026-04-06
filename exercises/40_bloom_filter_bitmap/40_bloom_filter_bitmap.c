#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * 20 位图实现 Bloom 过滤器
 * 需求：m=100 位图，k=3 个哈希函数；插入 "apple"、"banana"，查询示例
 */

typedef struct {
    unsigned char *bits;
    size_t m; /* 位图大小（比特） */
} Bloom;

static Bloom *bloom_create(size_t m) {
    Bloom *bf = (Bloom *)malloc(sizeof(Bloom));
    if (!bf) {
        return NULL;
    }
    
    size_t bytes = (m + 7) / 8;
    bf->bits = (unsigned char *)calloc(bytes, sizeof(unsigned char));
    if (!bf->bits) {
        free(bf);
        return NULL;
    }
    
    bf->m = m;
    return bf;
}

static void bloom_free(Bloom *bf) {
    if (bf) {
        if (bf->bits) {
            free(bf->bits);
        }
        free(bf);
    }
}

/* 位操作 */
static void set_bit(unsigned char *bm, size_t idx) {
    size_t byte = idx / 8;
    size_t offset = idx % 8;
    bm[byte] |= (1 << offset);
}
static int test_bit(const unsigned char *bm, size_t idx) {
    size_t byte = idx / 8;
    size_t offset = idx % 8;
    return (bm[byte] >> offset) & 1;
}

/* 三个简单哈希：sum(c*k) % m */
static size_t hash_k(const char *s, size_t m, int k) {
    size_t sum = 0;
    while (*s) {
        sum += (*s++) * k;
    }
    return sum % m;
}

static void bloom_add(Bloom *bf, const char *s) {
    set_bit(bf->bits, hash_k(s, bf->m, 1));
    set_bit(bf->bits, hash_k(s, bf->m, 2));
    set_bit(bf->bits, hash_k(s, bf->m, 3));
}

static int bloom_maybe_contains(Bloom *bf, const char *s) {
    if (!test_bit(bf->bits, hash_k(s, bf->m, 1))) {
        return 0;
    }
    if (!test_bit(bf->bits, hash_k(s, bf->m, 2))) {
        return 0;
    }
    if (!test_bit(bf->bits, hash_k(s, bf->m, 3))) {
        return 0;
    }
    return 1;
}

int main(void) {
    const size_t m = 100; /* 位图大小 */
    Bloom *bf = bloom_create(m);
    if (!bf) {
        fprintf(stderr, "bloom create failed\n");
        return 1;
    }

    /* 插入元素："apple"、"banana" */
    bloom_add(bf, "apple");
    bloom_add(bf, "banana");

    /* 查询元素："apple"（应存在）、"orange"（可能存在误判） */
    int apple = bloom_maybe_contains(bf, "apple");
    int grape = bloom_maybe_contains(bf, "grape"); /* 未插入，可能误判 */

    printf("apple exists: %d\n", apple); /* 必须为 1（无假阴性） */
    printf("grape exists: %d\n", grape); /* 允许 0 或 1（可能误判） */

    bloom_free(bf);
    return 0;
}
