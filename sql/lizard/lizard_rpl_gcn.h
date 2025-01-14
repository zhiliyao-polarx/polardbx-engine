/*****************************************************************************

Copyright (c) 2013, 2023, Alibaba and/or its affiliates. All Rights Reserved.

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License, version 2.0, as published by the
Free Software Foundation.

This program is also distributed with certain software (including but not
limited to OpenSSL) that is licensed under separate terms, as designated in a
particular file or component or in included license documentation. The authors
of MySQL hereby grant you an additional permission to link the program and
your derivative works with the separately licensed software that they have
included with MySQL.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU General Public License, version 2.0,
for more details.

You should have received a copy of the GNU General Public License along with
this program; if not, write to the Free Software Foundation, Inc.,
51 Franklin St, Fifth Floor, Boston, MA 02110-1301  USA

*****************************************************************************/

#ifndef LIZARD_LIZARD_RPL_GCN_INCLUDED
#define LIZARD_LIZARD_RPL_GCN_INCLUDED

#include "my_dbug.h"
#include "lizard_iface.h"

struct MyGCN {
 private:
  my_csr_t csr;
  my_gcn_t gcn;

 public:
  MyGCN() { reset(); }

  void set(const my_gcn_t _gcn, const my_csr_t _csr) {
    gcn = _gcn;
    csr = _csr;
  }

  bool is_empty() const {
#ifdef UNIV_DEBUG
    if (gcn == MYSQL_GCN_NULL) {
      DBUG_ASSERT(csr == MYSQL_CSR_NONE);
    }
#endif
    return gcn == MYSQL_GCN_NULL;
  }

  bool is_automatic() const {
#ifdef UNIV_DEBUG
    if (csr == MYSQL_CSR_AUTOMATIC) {
      DBUG_ASSERT(gcn != MYSQL_GCN_NULL);
    }
#endif
    return csr == MYSQL_CSR_AUTOMATIC;
  }

  bool is_assigned() const {
#ifdef UNIV_DEBUG
    if (csr == MYSQL_CSR_ASSIGNED) {
      DBUG_ASSERT(gcn != MYSQL_GCN_NULL);
    }
#endif
    return csr == MYSQL_CSR_ASSIGNED;
  }

  my_gcn_t get_gcn() const { return gcn; }

  my_csr_t get_csr() const { return csr; }

  void reset() {
    gcn = MYSQL_GCN_NULL;
    csr = MYSQL_CSR_NONE;
  }
};

#define MyGCN_NULL (MyGCN{})

struct MyVisionGCN {
 public:
  MyVisionGCN() { reset(); }

  void reset() {
    csr = MYSQL_CSR_NONE;
    gcn = MYSQL_GCN_NULL;
    current_scn = MYSQL_SCN_NULL;
  }

  void set(my_csr_t _csr, my_gcn_t _gcn, my_scn_t _scn) {
    DBUG_ASSERT(_csr == MYSQL_CSR_AUTOMATIC || _csr == MYSQL_CSR_ASSIGNED);
    if (_csr == MYSQL_CSR_ASSIGNED) {
      DBUG_ASSERT(_scn == MYSQL_SCN_NULL);
    } else {
      DBUG_ASSERT(_scn != MYSQL_SCN_NULL);
    }
    csr = _csr;
    gcn = _gcn;
    current_scn = _scn;
  }

  bool is_null() {
    if (csr == MYSQL_CSR_AUTOMATIC) {
      DBUG_ASSERT(gcn != MYSQL_GCN_NULL);
      DBUG_ASSERT(current_scn != MYSQL_SCN_NULL);
    } else if (csr == MYSQL_CSR_ASSIGNED) {
      DBUG_ASSERT(gcn != MYSQL_GCN_NULL);
      DBUG_ASSERT(current_scn == MYSQL_SCN_NULL);
    } else {
      DBUG_ASSERT(gcn == MYSQL_GCN_NULL);
      DBUG_ASSERT(current_scn == MYSQL_SCN_NULL);
    }

    return csr == MYSQL_CSR_NONE;
  }

  my_csr_t csr;
  my_gcn_t gcn;
  my_scn_t current_scn;
};

#endif
