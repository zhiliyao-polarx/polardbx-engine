/* Copyright (c) 2018, 2021, Alibaba and/or its affiliates. All rights reserved.
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License, version 2.0,
   as published by the Free Software Foundation.
   This program is also distributed with certain software (including
   but not limited to OpenSSL) that is licensed under separate terms,
   as designated in a particular file or component or in included license
   documentation.  The authors of MySQL/PolarDB-X Engine hereby grant you an
   additional permission to link the program and your derivative works with the
   separately licensed software that they have included with
   MySQL/PolarDB-X Engine.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License, version 2.0, for more details.
   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301  USA */

#ifndef SYS_VARS_EXT_H_INCLUDED
#define SYS_VARS_EXT_H_INCLUDED

/**
  @file
  "private" interface to sys_var - server configuration variables.

  This header is included by the file that contains declarations
  of extra sys_var variables (sys_vars_ext.cc).
*/

#include "my_config.h"

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

extern "C" MYSQL_PLUGIN_IMPORT char innodb_version[SERVER_VERSION_LENGTH];

extern void customize_server_version();

static constexpr uint DEFAULT_RPC_PORT = 33660;
extern int32 rpc_port;
extern bool new_rpc;

static constexpr ulonglong DEFAULT_IMPORT_TABLESPACE_ITERATOR_INTERVAL = 0;
extern ulonglong import_tablespace_iterator_interval;

#endif /* SYS_VARS_EXT_H_INCLUDED */
