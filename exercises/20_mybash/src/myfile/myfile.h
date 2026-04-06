#ifndef __MYFILE_H__
#define __MYFILE_H__

#include "common.h"

#include <elf.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


void print_elf_type(uint16_t e_type);
int __cmd_myfile(const char* filename);

#endif //! __MYFILE_H__