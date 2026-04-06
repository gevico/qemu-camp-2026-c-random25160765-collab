#include <elf.h>
#include <errno.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * 15 简易 ELF 信息查看工具
 * 功能：解析 ELF64 文件头，输出：类型、入口地址、所有 PT_LOAD 段的虚拟内存范围
 * 用法：
 *   - 无参数：默认解析 /bin/ls
 *   - 带 1 个参数：解析指定 ELF 文件
 */

/*
 * 字节序转换工具 bswap16/32/64
 * 若文件端序与主机端序不一致则对 ELF 头与 Program Header 分别做字段级转换，保证在 LE/BE 文件上解析正确
 */
static uint16_t bswap16(uint16_t v) { return (uint16_t)((v << 8) | (v >> 8)); }
static uint32_t bswap32(uint32_t v) {
    return ((v & 0x000000FFu) << 24) | ((v & 0x0000FF00u) << 8) | ((v & 0x00FF0000u) >> 8) | ((v & 0xFF000000u) >> 24);
}
static uint64_t bswap64(uint64_t v) {
    return ((uint64_t)bswap32((uint32_t)(v & 0xFFFFFFFFu)) << 32) |
           (uint64_t)bswap32((uint32_t)((v >> 32) & 0xFFFFFFFFu));
}

/* 检测主机是否小端（常见 x86_64 为小端） */
static int host_is_little_endian(void) {
    uint16_t x = 0x0102;
    return *((uint8_t *)&x) == 0x02; /* 小端则最低地址为低字节 0x02 */
}

/*
 * 读取 /usr/include/elf.h 中的 Elf64_Ehdr 结构体定义
 * 将 ELF 头字段按需要进行字节序转换（若文件端序与主机端序不同）
 */
static void fix_ehdr_endian(const Elf64_Ehdr *src, Elf64_Ehdr *dst, int file_is_le, int host_is_le) {
    // 复制整个结构体
    memcpy(dst, src, sizeof(Elf64_Ehdr));
    
    // 如果文件端序与主机端序不同，需要转换字节序
    if (file_is_le != host_is_le) {
        dst->e_type = bswap16(dst->e_type);
        dst->e_machine = bswap16(dst->e_machine);
        dst->e_version = bswap32(dst->e_version);
        dst->e_entry = bswap64(dst->e_entry);
        dst->e_phoff = bswap64(dst->e_phoff);
        dst->e_shoff = bswap64(dst->e_shoff);
        dst->e_flags = bswap32(dst->e_flags);
        dst->e_ehsize = bswap16(dst->e_ehsize);
        dst->e_phentsize = bswap16(dst->e_phentsize);
        dst->e_phnum = bswap16(dst->e_phnum);
        dst->e_shentsize = bswap16(dst->e_shentsize);
        dst->e_shnum = bswap16(dst->e_shnum);
        dst->e_shstrndx = bswap16(dst->e_shstrndx);
    }
}

static void fix_phdr_endian(const Elf64_Phdr *src, Elf64_Phdr *dst, int file_is_le, int host_is_le) {
    // 复制整个结构体
    memcpy(dst, src, sizeof(Elf64_Phdr));
    
    // 如果文件端序与主机端序不同，需要转换字节序
    if (file_is_le != host_is_le) {
        dst->p_type = bswap32(dst->p_type);
        dst->p_flags = bswap32(dst->p_flags);
        dst->p_offset = bswap64(dst->p_offset);
        dst->p_vaddr = bswap64(dst->p_vaddr);
        dst->p_paddr = bswap64(dst->p_paddr);
        dst->p_filesz = bswap64(dst->p_filesz);
        dst->p_memsz = bswap64(dst->p_memsz);
        dst->p_align = bswap64(dst->p_align);
    }
}

static const char *etype_to_str(uint16_t e_type) {
    switch (e_type) {
        case ET_NONE: /* 无类型 */
            return "ET_NONE";
        case ET_REL: /* 可重定位文件 */
            return "ET_REL";
        case ET_EXEC: /* 可执行文件 */
            return "ET_EXEC";
        case ET_DYN: /* 共享目标文件 */
            return "ET_DYN";
        case ET_CORE: /* 核心转储文件 */
            return "ET_CORE";
        default:
            return "Unknown";
    }
}

int main(int argc, char **argv) {
    /* 输入 ELF 文件路径，默认解析 /bin/ls，若有参数则解析指定文件 */
    const char *path = (argc >= 2) ? argv[1] : "/bin/ls";

    FILE *fp = fopen(path, "rb");
    if (!fp) {
        fprintf(stderr, "无法打开文件 %s: %s\n", path, strerror(errno));
        return 1;
    }

    /* 读取 Elf64_Ehdr */
    Elf64_Ehdr eh_src;
    if (fread(&eh_src, 1, sizeof(eh_src), fp) != sizeof(eh_src)) {
        fprintf(stderr, "读取 ELF 头失败\n");
        fclose(fp);
        return 1;
    }

    /* 校验 ELF 魔数与位宽 ELFCLASS64 / 端序 EI_DATA */
    if (memcmp(eh_src.e_ident, ELFMAG, SELFMAG) != 0) {
        fprintf(stderr, "不是有效的 ELF 文件\n");
        fclose(fp);
        return 1;
    }
    if (eh_src.e_ident[EI_CLASS] != ELFCLASS64) {
        fprintf(stderr, "当前仅支持 ELF64\n");
        fclose(fp);
        return 1;
    }
    int file_is_le = (eh_src.e_ident[EI_DATA] == ELFDATA2LSB);
    int host_is_le = host_is_little_endian();

    Elf64_Ehdr eh;
    fix_ehdr_endian(&eh_src, &eh, file_is_le, host_is_le);

    /* 定位 e_phoff，按 e_phentsize 与 e_phnum 遍历读取 Elf64_Phdr */
    if (eh.e_phoff == 0 || eh.e_phnum == 0) {
        fprintf(stderr, "ELF 中不存在程序头表\n");
        fclose(fp);
        return 1;
    }
    if (fseek(fp, (long)eh.e_phoff, SEEK_SET) != 0) {
        fprintf(stderr, "定位到程序头表失败\n");
        fclose(fp);
        return 1;
    }

    /* 安全性：限制最大读取数量，防止畸形文件导致巨大分配 */
    const size_t max_phnum = 4096;
    if (eh.e_phnum > max_phnum) {
        fprintf(stderr, "程序头数量过大，可能是畸形文件\n");
        fclose(fp);
        return 1;
    }

    /* 读取到临时缓冲，然后逐个进行端序修正 */
    size_t entsz = eh.e_phentsize;
    if (entsz < sizeof(Elf64_Phdr)) {
        /* 大多数系统上 e_phentsize == sizeof(Elf64_Phdr)，小于则无法安全解析 */
        fprintf(stderr, "程序头项大小异常：%zu\n", entsz);
        fclose(fp);
        return 1;
    }

    /* 逐项读取到 Elf64_Phdr 大小（读多余字节时截断处理） */
    int first = 1;
    printf("Type: %s, Entry: 0x%" PRIx64 ", Load segments: ", etype_to_str(eh.e_type),
           eh.e_entry); /* 输出 ELF 类型与入口地址 */
    for (uint16_t i = 0; i < eh.e_phnum; ++i) {
        /* 跳到第 i 项的开始位置 */
        if (fseek(fp, (long)(eh.e_phoff + (uint64_t)i * entsz), SEEK_SET) != 0) {
            fprintf(stderr, "\n读取程序头项定位失败\n");
            fclose(fp);
            return 1;
        }

        /* 读取最小必要大小 */
        Elf64_Phdr ph_src;
        memset(&ph_src, 0, sizeof(ph_src));
        size_t to_read = sizeof(Elf64_Phdr);
        size_t readn = fread(&ph_src, 1, to_read, fp);
        if (readn < to_read) {
            fprintf(stderr, "\n读取程序头项失败\n");
            fclose(fp);
            return 1;
        }

        /* 过滤 PT_LOAD，输出加载段 p_vaddr 到 p_vaddr + p_memsz 的虚拟地址范围，多个段用逗号分隔 */
        Elf64_Phdr ph;
        fix_phdr_endian(&ph_src, &ph, file_is_le, host_is_le);
        if (ph.p_type == PT_LOAD) {
            if (!first) {
                printf(", ");
            }
            first = 0;
            uint64_t start = ph.p_vaddr;
            uint64_t end = ph.p_vaddr + ph.p_memsz;
            printf("0x%" PRIx64 "-0x%" PRIx64, start, end);
        }
    }
    if (first) {
        /* 没有任何 PT_LOAD 段 */
        printf("(none)");
    }
    printf("\n");

    fclose(fp);
    return 0;
}
