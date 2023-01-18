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
//#include "diskio.h"		/* Declarations of device I/O functions */

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

static mrb_value mrb_fatfs_create(mrb_state *mrb, mrb_value self)
{
  mrb_fatfs_data *data = DATA_PTR(self);
  mrb_value arry;
  char *path;
  char buffer[512];
  mrb_int len;
  int alen;
  FIL fi;
  UINT bw;
  int i, sec;

  mrb_get_args(mrb, "sA", &path, &len, &arry);
  alen = RARRAY_LEN( arry );

  f_open(&fi, path, FA_CREATE_ALWAYS | FA_WRITE);
  sec = 0;
  while (alen != 0) {
    for (i = 0; i < 512; ++i) {
      buffer[i] = mrb_fixnum( mrb_ary_ref( mrb, arry, i + sec * 512) );
      --alen;
      if (alen == 0)
        break;
    }
    if (i != 512)
      ++i;
    f_write(&fi, buffer, i, &bw);
    ++sec;
  }
  f_close(&fi);

  return mrb_fixnum_value(0);

}

static mrb_value mrb_fatfs_read(mrb_state *mrb, mrb_value self)
{
  mrb_fatfs_data *data = DATA_PTR(self);
  mrb_value res;
  char *path;
  char buffer[512];
  mrb_int len;
  FIL fi;
  UINT br;
  int i;

  mrb_get_args(mrb, "s", &path, &len);

  f_open(&fi, path, FA_OPEN_EXISTING | FA_READ);

  res = mrb_ary_new(mrb);
  for (;;) {
    f_read(&fi, buffer, sizeof buffer, &br);
    if (br == 0)
      break;
    for (i = 0; i < br; ++i) {
      mrb_ary_push(mrb, res, mrb_fixnum_value(buffer[i]));
    }
  }
  f_close(&fi);

  return res;
}

void mrb_mruby_fatfs_gem_init(mrb_state *mrb)
{
  struct RClass *fatfs;
  fatfs = mrb_define_class(mrb, "FatFs", mrb->object_class);
  mrb_define_method(mrb, fatfs, "initialize", mrb_fatfs_init, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, fatfs, "dir", mrb_fatfs_dir, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, fatfs, "create", mrb_fatfs_create, MRB_ARGS_REQ(2));
  mrb_define_method(mrb, fatfs, "read", mrb_fatfs_read, MRB_ARGS_REQ(1));
  DONE;
}

void mrb_mruby_fatfs_gem_final(mrb_state *mrb)
{

  close(fatfd);
}

