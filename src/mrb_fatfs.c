/*
** mrb_fatfs.c - FatFs class
**
** Copyright (c) Hiroki Mori 2023
**
** See Copyright Notice in LICENSE
*/

#include <fcntl.h>
#include <unistd.h>

#include "mruby.h"
#include "mruby/data.h"
#include "mruby/array.h"
#include "mrb_fatfs.h"

#include "ff.h"			/* Declarations of FatFs API */
#include "diskio.h"		/* Declarations of device I/O functions */

#define DONE mrb_gc_arena_restore(mrb, 0);

typedef struct {
  char *str;
  mrb_int len;
  FATFS fs;
} mrb_fatfs_data;

static const struct mrb_data_type mrb_fatfs_data_type = {
  "mrb_fatfs_data", mrb_free,
};

int fatfd;

static mrb_value mrb_fatfs_init(mrb_state *mrb, mrb_value self)
{
  mrb_fatfs_data *data;
  char *str;
  mrb_int len;

  data = (mrb_fatfs_data *)DATA_PTR(self);
  if (data) {
    mrb_free(mrb, data);
  }
  DATA_TYPE(self) = &mrb_fatfs_data_type;
  DATA_PTR(self) = NULL;

  mrb_get_args(mrb, "s", &str, &len);
  data = (mrb_fatfs_data *)mrb_malloc(mrb, sizeof(mrb_fatfs_data));
  data->str = str;
  data->len = len;
  DATA_PTR(self) = data;

  fatfd = open(str, O_RDWR);

  f_mount(&(data->fs), "0:", 1);

  return self;
}

static mrb_value mrb_fatfs_dir(mrb_state *mrb, mrb_value self)
{
  mrb_fatfs_data *data = DATA_PTR(self);
  mrb_value resary;
  char *path;
  mrb_int len;

  mrb_get_args(mrb, "s", &path, &len);

  resary = mrb_ary_new(mrb);

  DIR dir;
  FILINFO fno;
  FRESULT res;
  res = f_opendir(&dir, path);
  if (res == FR_OK) {
    for (;;) {
      res = f_readdir(&dir, &fno);
      if (res != FR_OK || fno.fname[0] == 0) break;
      mrb_ary_push(mrb, resary,  mrb_str_new_cstr(mrb, fno.fname));
    }
  }

  return resary;
}

void mrb_mruby_fatfs_gem_init(mrb_state *mrb)
{
  struct RClass *fatfs;
  fatfs = mrb_define_class(mrb, "FatFs", mrb->object_class);
  mrb_define_method(mrb, fatfs, "initialize", mrb_fatfs_init, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, fatfs, "dir", mrb_fatfs_dir, MRB_ARGS_REQ(1));
  DONE;
}

void mrb_mruby_fatfs_gem_final(mrb_state *mrb)
{

  close(fatfd);
}

